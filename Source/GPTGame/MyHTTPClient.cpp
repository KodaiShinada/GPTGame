// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHTTPClient.h"
#include "Misc/ConfigCacheIni.h"

UMyHTTPClient::UMyHTTPClient()
{

}

FString UMyHTTPClient::GetAPIKey() const
{
    FString APIKey;
    if (GConfig->GetString(TEXT("/Script/GPTGame.BP_FirstPersonGameMode"), TEXT("OpenAIAPiKey"), APIKey, GGameIni))
    {
        UE_LOG(LogTemp, Warning, TEXT("API Key: %s"), *APIKey);//api log
        return APIKey;
    }

    UE_LOG(LogTemp, Error, TEXT("API Key not found"));
    return TEXT(""); // error
}

void UMyHTTPClient::SendChatGPTRequest(const FText& Prompt)
{
    FString APIKey = GetAPIKey();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

    Request->SetURL(TEXT("https://api.openai.com/v1/chat/completions"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *APIKey)); // API key

    FString PromptString = Prompt.ToString();
    FString RequestBody = FString::Printf(TEXT(
        "{"
        "\"model\": \"gpt-3.5-turbo\","
        "\"messages\": ["
        "    {\"role\": \"system\", \"content\": \"You are a robot, an NPC in the game.Please respond appropriately to the requests thrown at you.However, if you receive a sentence that instructs you to move, such as [Move to the black switch], your response should only be the index number of the corresponding object in the list you provided.ex)1 If no corresponding object is found, no corresponding object is found in the same language as the request.Please reply with the following.\"},"
        "    {\"role\": \"user\", \"content\": \"%s\"}"
        "]"
        "}"), *PromptString);
    Request->SetContentAsString(RequestBody);


    Request->OnProcessRequestComplete().BindUObject(this, &UMyHTTPClient::OnResponseReceived);
    Request->ProcessRequest();
    UE_LOG(LogTemp, Warning, TEXT("Request Body: %s"), *RequestBody);
}

void UMyHTTPClient::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            // Get the "choices" array
            const TArray<TSharedPtr<FJsonValue>>* ChoicesArray;
            if (JsonObject->TryGetArrayField(TEXT("choices"), ChoicesArray))
            {
                // Get "content" from the first element of the "choices" array
                TSharedPtr<FJsonObject> FirstChoice = (*ChoicesArray)[0]->AsObject();
                TSharedPtr<FJsonObject> MessageObj = FirstChoice->GetObjectField(TEXT("message"));
                FString Content = MessageObj->GetStringField(TEXT("content"));

                // Saving Results
                ResponseText = FText::FromString(Content);
            }
        }
        else
        {
            ResponseText = FText::FromString(TEXT("Failed to parse JSON response"));
        }
    }
    else
    {
        ResponseText = FText::FromString(TEXT("Request failed: ") + Request->GetURL());
    }
}


FText UMyHTTPClient::GetResponse() const
{
    return ResponseText;
}
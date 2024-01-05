// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHTTPClient.h"

UMyHTTPClient::UMyHTTPClient()
{

}

void UMyHTTPClient::SendChatGPTRequest(const FString& Prompt)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

    Request->SetURL(TEXT("https://api.openai.com/v1/chat/completions"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Authorization"), TEXT("Bearer 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")); // APIƒL[

    FString RequestBody = FString::Printf(TEXT(
        "{"
        "\"model\": \"gpt-3.5-turbo\","
        "\"messages\": ["
        "    {\"role\": \"system\", \"content\": \"You are a helpful assistant.\"},"
        "    {\"role\": \"user\", \"content\": \"%s\"}"
        "]"
        "}"), *Prompt);
    Request->SetContentAsString(RequestBody);


    Request->OnProcessRequestComplete().BindUObject(this, &UMyHTTPClient::OnResponseReceived);
    Request->ProcessRequest();
    UE_LOG(LogTemp, Warning, TEXT("Request Body: %s"), *RequestBody);
}

void UMyHTTPClient::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        ResponseString = Response->GetContentAsString();
    }
    else
    {
        ResponseString = TEXT("Failed to get response");
    }
}

FString UMyHTTPClient::GetResponse() const
{
    return ResponseString;
}
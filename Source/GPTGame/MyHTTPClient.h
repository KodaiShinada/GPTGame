#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "MyHTTPClient.generated.h"

UCLASS(Blueprintable)
class GPTGAME_API UMyHTTPClient : public UObject
{
    GENERATED_BODY()

public:
    UMyHTTPClient();

    UFUNCTION(BlueprintCallable, Category = "ChatGPT")
    void SendChatGPTRequest(const FText& Prompt);

    UFUNCTION(BlueprintCallable, Category = "ChatGPT")
    FText GetResponse() const;

    UFUNCTION(BlueprintCallable, Category = "ChatGPT")
    void SetAPIKey(const FString& NewAPIKey)
    {
        APIKey = NewAPIKey;
    };

protected:
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:
    FText ResponseText;
    FString APIKey;
};


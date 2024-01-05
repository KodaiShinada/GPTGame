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
    void SendChatGPTRequest(const FString& Prompt);

    UFUNCTION(BlueprintCallable, Category = "ChatGPT")
    FString GetResponse() const;

protected:
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:
    FString ResponseString;
};


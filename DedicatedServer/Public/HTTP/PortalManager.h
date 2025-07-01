// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HTTP/HTTPRequestManager.h"
#include "DedicatedServer/HTTPRequestTypes.h"
#include "Interfaces/PortalManageInterface.h"
#include "PortalManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DEDICATEDSERVER_API UPortalManager : public UHTTPRequestManager, public IPortalManageInterface
{
	GENERATED_BODY()
	
public:
	// Portal Management Interface
	virtual void RefreshToken(const FString& RefreshToken) override;
	virtual void SignOut(const FString& AccessToken) override;
	
	void SignIn(const FString& UserName, const FString& Password);
	void SignUp(const FString& UserName, const FString& Password, const FString& Email);
	void ConfirmEmail(const FString& ConfrimCode);
	FORCEINLINE const FDSSignUpResponse& GetLastSignUpResponse() const { return LastSignUpResponse; }

	UPROPERTY(BlueprintAssignable)
	FStatusMessageDelegate SignInStatusMessageDelegate;

	UPROPERTY(BlueprintAssignable)
	FStatusMessageDelegate SignUpStatusMessageDelegate;

	UPROPERTY(BlueprintAssignable)
	FStatusMessageDelegate ConfirmEmailStatusMessageDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAPIRequestSucceeded OnSignInSucceeded;

	UPROPERTY(BlueprintAssignable)
	FOnAPIRequestSucceeded OnSignUpSucceeded;

	UPROPERTY(BlueprintAssignable)
	FOnAPIRequestSucceeded OnConfirmEmailSucceeded;

	UFUNCTION()
	void QuitGame();

protected:


private:
	void SignIn_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void SignUp_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void Confirm_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void SignOut_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void RefreshToken_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	FDSSignUpResponse LastSignUpResponse;
	FString CachedUserName;
	FString LastSignUpUserName;

};

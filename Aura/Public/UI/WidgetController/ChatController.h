// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "ChatController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChatReceived, const FString&, SenderName, const FString&, Chat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChatModeOn);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AURA_API UChatController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UFUNCTION(BlueprintCallable)
	void SendChat(const FString& Chat);

	UPROPERTY(BlueprintAssignable)
	FOnChatReceived OnChatReceived;

	UPROPERTY(BlueprintAssignable)
	FChatModeOn OnChatModeOn;

protected:

	UPROPERTY()
	TObjectPtr<class UChatComponent> ChatComponent;

private:
	
	TObjectPtr<class AAuraPlayerController> AuraPC;
};

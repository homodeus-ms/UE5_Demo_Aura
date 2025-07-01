// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChatReceivedDelegate, const FString& /* SenderName */, const FString& /* Chat */);
DECLARE_MULTICAST_DELEGATE(FChatModeOnDelegate);
DECLARE_MULTICAST_DELEGATE(FChatModeOffDelegate);
DECLARE_DELEGATE_RetVal(FString, FGetCurrentLevelNameDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API UChatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UChatComponent();
	
	void SetChatModeOn();
	void SendChat(const FString& Message);
	void SetUserName(const FString& UserName);
	
	UFUNCTION(Client, Reliable)
	void Client_BroadcastChatToWidget(const FString& InSenderName, const FString& Chat);

	FOnChatReceivedDelegate OnChatReceived;

	FOnChatReceivedDelegate OnChatReceivedInServer;

	// After Enter Key Pressed, PlayerController -> ... -> UI
	FChatModeOnDelegate OnChatModeOn; 

	// After Enter Ket Pressed, UI -> ... -> PlayerController
	FChatModeOffDelegate OnChatModeOff; 

	FGetCurrentLevelNameDelegate GetCurrentLevelNameDelegate;

protected:
	
	UFUNCTION(Server, Reliable)
	void Server_Chat(const FString& InSenderName, const FString& InChat);

	bool IsShorterThanMaxLength(const FString& Message);

private:
	class UDS_LocalPlayerSubsystem* GetDSLocalPlayerSubSystem();
	bool ConvertSpecialKeyword(const FString& Word, FString& OUTConverted);

	enum { CHAT_MAX_LENGTH = 230 };
	FString SenderName{};
	
};

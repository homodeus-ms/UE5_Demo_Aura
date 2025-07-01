// Fill out your copyright notice in the Description page of Project Settings.


#include "Chat/ChatComponent.h"
#include "DedicatedServer/Public/Player/DS_LocalPlayerSubsystem.h"
#include "AuraLog.h"

UChatComponent::UChatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UChatComponent::SetChatModeOn()
{
	UE_LOG(Aura, Log, TEXT("UChatComponent::SetChatModeOn()"));
	OnChatModeOn.Broadcast();
}


void UChatComponent::SetUserName(const FString& UserName)
{
	UE_LOG(Aura, Log, TEXT("UChatComponent::SetUserName(), InParam: %s"), *UserName);
	if (UserName.IsEmpty())
	{
		SenderName = TEXT("UnknownUser"); 
		return;
	}
	SenderName = UserName;
}

void UChatComponent::SendChat(const FString& Message)
{
	check(!SenderName.IsEmpty());

	UE_LOG(Aura, Log, TEXT("UChatComponent::SendChat, %s"), *Message);

	// Handle Special Keyword
	FString ConvertString{};
	if (ConvertSpecialKeyword(Message, ConvertString))
	{
		// Don't neee to Broadcast To All Client
		OnChatModeOff.Broadcast();
		OnChatReceived.Broadcast(TEXT(""), ConvertString);
		return;
	}

	if (!Message.IsEmpty())
	{
		if (IsShorterThanMaxLength(Message))
		{
			Server_Chat(SenderName, Message);
		}
		else
		{
			FString CutMessage = Message.Left(CHAT_MAX_LENGTH);
			Server_Chat(SenderName, CutMessage);
		}
	}

	OnChatModeOff.Broadcast();
}


void UChatComponent::Client_BroadcastChatToWidget_Implementation(const FString& InSenderName, const FString& Chat)
{
	UE_LOG(Aura, Log, TEXT("UChatComponent::Client_BroadcastChatToWidget"));
	OnChatReceived.Broadcast(InSenderName, Chat);
}

bool UChatComponent::IsShorterThanMaxLength(const FString& Message)
{
	return Message.Len() < CHAT_MAX_LENGTH;
}

bool UChatComponent::ConvertSpecialKeyword(const FString& Word, FString& OUTConverted)
{
	if (Word.IsEmpty())
		return false;

	if (Word == TEXT("/Map") || Word == TEXT("/map"))
	{
		UE_LOG(Aura, Log, TEXT("ConvertSpecialKeyword, /Map"));
		
		if (GetCurrentLevelNameDelegate.IsBound())
		{
			const FString& MapName = GetCurrentLevelNameDelegate.Execute();
			OUTConverted = FString::Printf(TEXT("--- %s is in %s"), *SenderName, *MapName);
			return true;
		}
	}

	return false;
}


void UChatComponent::Server_Chat_Implementation(const FString& InSenderName, const FString& InChat)
{
	check(InSenderName.Len() + InChat.Len() < CHAT_MAX_LENGTH);

	UE_LOG(Aura, Log, TEXT("Sender: %s"), *InSenderName);
	UE_LOG(Aura, Log, TEXT("Chat: %s"), *InChat);

	OnChatReceivedInServer.Broadcast(InSenderName, InChat);
}

UDS_LocalPlayerSubsystem* UChatComponent::GetDSLocalPlayerSubSystem()
{
	UE_LOG(Aura, Log, TEXT("UChatComponent::GetDSLocalPlayerSubSystem()"));

	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!PC)
	{
		UE_LOG(Aura, Log, TEXT("PC Is Null!"));
		return nullptr;
	}

	UDS_LocalPlayerSubsystem* Subsystem = PC->GetLocalPlayer()->GetSubsystem<UDS_LocalPlayerSubsystem>();
	
	return Subsystem;
}


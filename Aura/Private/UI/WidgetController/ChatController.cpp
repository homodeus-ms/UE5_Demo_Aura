// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/ChatController.h"
#include "Player/AuraPlayerController.h"
#include "Chat/ChatComponent.h"
#include "AuraLog.h"

void UChatController::BroadcastInitialValues()
{
}

void UChatController::BindCallbacksToDependencies()
{
	check(PlayerController);
	AuraPC = Cast<AAuraPlayerController>(PlayerController);
	check(AuraPC);
	ChatComponent = AuraPC->GetChatComponent();

	ChatComponent->OnChatReceived.AddLambda([this](const FString& SenderName, const FString& Chat)
		{
			UE_LOG(Aura, Log, TEXT("UChatController::ChatReceived Callback!"));
			OnChatReceived.Broadcast(SenderName, Chat);
		});

	ChatComponent->OnChatModeOn.AddLambda([this]() 
		{
			UE_LOG(Aura, Log, TEXT("UChatController::SetChatModeOn Callback!"));
			OnChatModeOn.Broadcast();
		});
}

void UChatController::SendChat(const FString& Chat)
{
	ChatComponent->SendChat(Chat);
}


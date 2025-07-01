// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"
#include "Player/AuraPlayerState.h"
#include "AuraGameplayTags.h"
#include "AuraLog.h"

UInventoryWidgetController::UInventoryWidgetController()
{
}

void UInventoryWidgetController::BroadcastInitialValues()
{
	OnGoldChanged.Broadcast(AuraPlayerState->GetGold());
	const TMap<FGameplayTag, int32>& Items = AuraPlayerState->GetInventoryItems();
	for (const auto& Pair : Items)
	{
		OnInventoryChanged.Broadcast(Pair.Key, Pair.Value);
	}
}

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	AuraPlayerState->OnGoldChangedDelegate.AddUObject(this, &UInventoryWidgetController::OnGoldChangedCallback);
	AuraPlayerState->OnInventoryChangedDelegate.AddUObject(this, &UInventoryWidgetController::OnInventoryChangedCallback);
}

void UInventoryWidgetController::BuyItem(TArray<TPair<FGameplayTag, int32>> Items, int32 SpentGold)
{
	AuraPlayerState->BuyItem(Items, SpentGold);
}

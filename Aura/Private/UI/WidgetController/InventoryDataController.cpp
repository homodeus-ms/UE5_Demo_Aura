// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryDataController.h"
#include "Player/AuraPlayerState.h"
#include "AuraLog.h"

void UInventoryDataController::BroadcastInitialValues()
{
	
}

void UInventoryDataController::BindCallbacksToDependencies()
{
	
}

void UInventoryDataController::PurchaseItems(const TArray<FInventoryItemEntry>& Items, int32 TotalPrice)
{
	AuraPlayerState->Server_PurchaseItem(Items, TotalPrice);

	/*AuraPlayerState->AddGold(-TotalPrice);
	for (const auto& Pair : Items)
	{
		AuraPlayerState->AddInventoryItemByItemSpendTag(Pair.Key, Pair.Value);
	}*/
}

void UInventoryDataController::OnGoldChangedCallback(int32 NewGold)
{
	OnGoldChanged.Broadcast(NewGold);
}

void UInventoryDataController::OnInventoryChangedCallback(const FGameplayTag& ItemTag, int32 Amount)
{
	OnInventoryChanged.Broadcast(ItemTag, Amount);
}

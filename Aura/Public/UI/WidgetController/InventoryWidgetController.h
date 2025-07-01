// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/InventoryDataController.h"
#include "InventoryWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UInventoryWidgetController : public UInventoryDataController
{
	GENERATED_BODY()
	
public:

	// Inventory Data Provider Implements

	UInventoryWidgetController();

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	// Buy Item
	void BuyItem(TArray<TPair<FGameplayTag, int32>> Items, int32 SpentGold);


protected:


};

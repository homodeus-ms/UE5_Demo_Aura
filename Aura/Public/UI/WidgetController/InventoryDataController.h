// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "AuraTypes.h"
#include "InventoryDataController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoldChanged, int32, Gold);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryChanged, const FGameplayTag&, ItemTag, int32, Amount);


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UInventoryDataController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|LevelUp")
	FOnGoldChanged OnGoldChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|LevelUp")
	FOnInventoryChanged OnInventoryChanged;

	UFUNCTION(BlueprintCallable)
	void PurchaseItems(const TArray<FInventoryItemEntry>& Items, int32 TotalPrice);

	

protected:
	UFUNCTION()
	void OnGoldChangedCallback(int32 NewGold);

	UFUNCTION()
	void OnInventoryChangedCallback(const FGameplayTag& ItemTag, int32 Amount);
};

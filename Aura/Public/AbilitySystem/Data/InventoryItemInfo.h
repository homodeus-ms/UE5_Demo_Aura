// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "InventoryItemInfo.generated.h"


USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ItemSpendTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ItemTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABuyableItem> ItemClass = nullptr;
};

/**
 * 
 */
UCLASS()
class AURA_API UInventoryItemInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	FInventoryItem FindItemBySpendTag(const FGameplayTag& ItemSpendTag);
	FInventoryItem FindItemByItemTag(const FGameplayTag& ItemTag);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InventoryItemInfo")
	TArray<FInventoryItem> InventoryItems;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "CollectingItemInfo.generated.h"

USTRUCT(BlueprintType)
struct FCollectingItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ItemTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ItemName = FString();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* ItemImage = nullptr;
};

/**
 * 
 */
UCLASS()
class AURA_API UCollectingItemInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UCollectingItemInfo();

	const TArray<FCollectingItem>& GetItemDataArray() const { return CollectingItems; }


protected:
	UPROPERTY(EditDefaultsOnly, Category="CollectingItems")
	TArray<FCollectingItem> CollectingItems;
};

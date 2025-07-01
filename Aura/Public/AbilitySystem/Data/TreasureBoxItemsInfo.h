// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "TreasureBoxItemsInfo.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UTreasureBoxItemsInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInfo")
	TArray<FGameplayTag> ItemTags;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/InventoryItemInfo.h"
#include "AbilitySystem/GA/AuraGABase.h"
#include "LoadScreenSaveGame.generated.h"


UENUM()
enum ESlotStatus
{
	Empty,
	EnterName,
	ShowInfoAndPlay
};

USTRUCT()
struct FStatSaveInfo
{
	GENERATED_BODY()

	UPROPERTY()
	int32 PlayerLevel = 1;

	UPROPERTY()
	int32 XP = 0;

	UPROPERTY()
	int32 SpellPoint = 0;

	UPROPERTY()
	int32 AttributePoint = 0;

	// Primary Attribute
	UPROPERTY()
	float Strength = 0.f;

	UPROPERTY()
	float Intelligence = 0.f;

	UPROPERTY()
	float Resilience = 0.f;

	UPROPERTY()
	float Vigor = 0.f;

	UPROPERTY()
	float CurrentHP = 0.f;

	UPROPERTY()
	float CurrentMP = 0.f;
};

USTRUCT()
struct FInventorySaveInfo
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Gold = 0;

	UPROPERTY()
	TMap<FGameplayTag, int32> InventoryItemAmounts;

	UPROPERTY()
	TArray<FGameplayTag> Collections;
};

USTRUCT()
struct FSkillSaveInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<class UGameplayAbility> AbilityClass;

	UPROPERTY()
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY()
	FGameplayTag AbilitySlot = FGameplayTag();

	UPROPERTY()
	FGameplayTag AbilityStatus = FGameplayTag();

	UPROPERTY()
	FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY()
	int32 AbilityLevel = 1;
};

inline bool operator==(const FSkillSaveInfo & Left, const FSkillSaveInfo & Right)
{
	return Left.AbilityTag.MatchesTagExact(Right.AbilityTag);
}

/**
 * 
 */
UCLASS()
class AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	FString SlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	FString PlayerName = FString("Default Player Name");

	UPROPERTY()
	FString MapName = FString("Default Map");

	UPROPERTY()
	FName MapAssetName = FName();

	UPROPERTY()
	TEnumAsByte<ESlotStatus> SlotStatus = ESlotStatus::Empty;

	UPROPERTY()
	FName PlayerStartTag = FName();
	
	UPROPERTY()
	FStatSaveInfo StatSaveInfo = FStatSaveInfo();

	UPROPERTY()
	bool bFirstLoadIn = true;

	UPROPERTY()
	TArray<FSkillSaveInfo> SavedAbilities;

	UPROPERTY()
	FInventorySaveInfo InventoryInfo = FInventorySaveInfo();

};

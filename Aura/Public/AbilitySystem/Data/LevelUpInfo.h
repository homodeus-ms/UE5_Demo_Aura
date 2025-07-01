// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePoint = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 SpellPoint = 0;
};

UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	const FAuraLevelUpInfo& GetLevelUpInfo(int32 Level) const;
	
	int32 GetCurrentLevelMaxXP(int32 Level) const;
	int32 GetMaxXP() const;
	int32 GetMaxLevel() { return LevelUpInfos.Num(); }
	int32 FindLevelForXP(int32 XP);
	
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="LevelUpInfo")
	TArray<FAuraLevelUpInfo> LevelUpInfos;
};

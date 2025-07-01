// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StageClearCondition.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FStageClearInfo
{
	GENERATED_BODY()

	bool operator==(const FStageClearInfo& Other) const
	{
		return TriggerCount == Other.TriggerCount && EnemyKillCount == Other.EnemyKillCount;
	}
	void Reset()
	{
		TriggerCount = 0;
		EnemyKillCount = 0;
	}

	UPROPERTY(EditDefaultsOnly)
	int32 TriggerCount = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 EnemyKillCount = 0;
};

UCLASS()
class AURA_API UStageClearCondition : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FStageClearInfo> StageClearInfos;
};

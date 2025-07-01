// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpawnFactoryInfo.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EFactorySpawnActorType : uint8
{
	TreasureBox,
	HealthPotion,
	ManaPotion,
	None,
};

UCLASS()
class AURA_API USpawnFactoryInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	const TMap<EFactorySpawnActorType, TSubclassOf<AActor>>& GetFactoryInfoMap() const { return FactoryInfo; }

protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<EFactorySpawnActorType, TSubclassOf<AActor>> FactoryInfo;
};

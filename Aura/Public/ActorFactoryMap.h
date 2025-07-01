// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/SpawnFactoryInfo.h"
#include "ActorFactoryMap.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class AURA_API UActorFactoryMap : public UObject
{
	GENERATED_BODY()
	
public:
	
	void RegisterMap();
	void SpawnAtLocation(UObject* WorldObject, EFactorySpawnActorType Type, const FVector& SpawnLocation, FRotator Rotator = FRotator::ZeroRotator);

protected: 

	UPROPERTY(EditDefaultsOnly, Category = "NeedToSetup|Default")
	TObjectPtr<class USpawnFactoryInfo> SpawnFactoryInfo;

	UPROPERTY()
	TMap<EFactorySpawnActorType, TSubclassOf<AActor>> TypeToClassMap;

private:
	
};

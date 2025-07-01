// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Interface/StageClearObserver.h"
#include "AuraGameUtility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "UtilityFunction", meta = (DefaultToSelf = "WorldContext"))
	static void GetSpawnLocation(const UObject* WorldContext, const FVector& Center, float Range, int32 SpawnCount, TArray<FVector>& OUTLocations, bool bRandInRange = true);

	UFUNCTION(BlueprintCallable, Category = "StageClear")
	static TScriptInterface<IStageClearObserver> GetStageClearObserver(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "FactoryMap")
	static class UActorFactoryMap* GetFactoryMap(const UObject* WorldContext);
};

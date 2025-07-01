// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GA/AuraTargetedGA.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraTargetedGA
{
	GENERATED_BODY()
	
public:

	virtual void ExecutePlayMontageAndWait() override;

	UFUNCTION(BlueprintCallable)
	void SpawnMinions(const TArray<FVector>& SpawnLocations);

	UFUNCTION(BlueprintCallable)
	void SpawnSpotEffect(const TArray<FVector>& SpawnLocations);

	
protected:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	UPROPERTY(EditAnywhere, Category="Summon")
	TArray<TSubclassOf<APawn>> SpawnMinionClasses;

	UPROPERTY(EditAnywhere, Category="Summon")
	TObjectPtr<class UNiagaraSystem> SpawnSpotRing;

	UPROPERTY(EditDefaultsOnly, Category = "Summon", meta = (ClampMin = "0", ClampMax = "5"))
	int32 MaxSpawnCount = 5;

	UPROPERTY(EditAnywhere, Category = "Summon")
	float SpreadAngle = 120.f;

	UPROPERTY(EditAnywhere, Category = "Summon")
	float RandomMinSpawnDist = 150.f;

	UPROPERTY(EditAnywhere, Category = "Summon")
	float RandomMaxSpawnDist = 450.f;

	UPROPERTY(EditAnywhere, Category = "Summon")
	uint8 MinMinionCountThreshold = 1;


};

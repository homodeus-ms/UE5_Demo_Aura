// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GA/AuraSpawnProjectile.h"
#include "AuraGA_Firebolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGA_Firebolt : public UAuraSpawnProjectile
{
	GENERATED_BODY()
	
public:
	UAuraGA_Firebolt();

	virtual FString GetDescription(const FString& SkillName, int32 Level = 1) override;
	virtual FString GetNextLevelDescription(const FString& SkillName, int32 Level = 1) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& InTargetLocation, const FGameplayTag& CombatSocketTag);

protected:

	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	float SpreadAngle = 90.f;

	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	int32 MaxNumOfProjectile = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	float MinAcceleration = 1800.f;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float MaxAcceleration = 2400.f;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	bool bHomingProjectile = true;
};

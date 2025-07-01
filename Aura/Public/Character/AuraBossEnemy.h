// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraEnemy.h"
#include "AuraBossEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraBossEnemy : public AAuraEnemy
{
	GENERATED_BODY()
	
public:
	AAuraBossEnemy(const FObjectInitializer& ObjectInitializer);

	virtual void OnComboAttackCompleted_Implementation() override;
	
protected:
	virtual void Die() override;

private:
	TArray<FVector> GetRewardBoxPoints();

	int32 RewardBoxCount = 0;

	FTimerHandle SpawnRewardBoxDelayTimer;

	float SpawnDelayTime = 2.f;
};

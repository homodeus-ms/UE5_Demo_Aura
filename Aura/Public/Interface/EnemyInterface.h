// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IEnemyInterface
{
	GENERATED_BODY()

public:

	virtual float GetDetactRange() const abstract;
	virtual float GetAttackRange() const abstract;
	virtual int32 GetRewardPoint() const abstract;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetTargetActor() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetTargetActor(AActor* InTargetActior);

	virtual void SetIsSpawnBySummon(bool IsTrue) abstract;
	virtual bool IsSpawnBySummon() const abstract;

	virtual class UWidgetComponent* GetHpBarComponent() abstract;
	virtual void AddMinionCount(int32 Count) abstract;
	virtual int32 GetCurrentMinionCount() const abstract;

	UFUNCTION()
	virtual void OnMinionDeadCallback(AActor* DestroyedActor) abstract;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "EnemySpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

	UPROPERTY(EditAnywhere, Category="EnemySpawn")
	TSubclassOf<class AAuraEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, Category="EnemySpawn")
	int32 EnemyLevel = 1;

	UPROPERTY(EditAnywhere, Category="EnemySpawn")
	ECharacterType EnemyType = ECharacterType::Warrior;

protected:
	

};

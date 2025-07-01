// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume/EnemySpawnPoint.h"
#include "Character/AuraEnemy.h"

void AEnemySpawnPoint::SpawnEnemy()
{
	if (!HasAuthority())
		return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(EnemyClass, GetActorTransform());
	Enemy->SetLevel(EnemyLevel);
	Enemy->SetCharacterType(EnemyType);
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}

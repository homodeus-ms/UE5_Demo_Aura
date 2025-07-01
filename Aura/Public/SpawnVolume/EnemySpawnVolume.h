// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnVolume.generated.h"

UCLASS()
class AURA_API AEnemySpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnVolume();

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> BoxTrigger;

	UPROPERTY(EditAnywhere, Category="SpawnVolume")
	TArray<TObjectPtr<class AEnemySpawnPoint>> SpawnPoints;
};

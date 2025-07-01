// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointCollection.generated.h"

UCLASS()
class AURA_API APointCollection : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APointCollection();

	TArray<USceneComponent*> GetSpawnLocation(const FVector& Center, int32 PointsNum, float RotateYaw = 0.f);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Transient)
	TArray<TObjectPtr<class USceneComponent>> SpawnPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 MaxSpawnPoint = 10;

};

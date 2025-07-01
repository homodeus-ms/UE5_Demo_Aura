// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameUtility.h"
#include "GameFramework/GameState.h"
#include "Game/AuraGameStateBase.h"
#include "Game/AuraGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Game/AuraGameModeBase.h"
#include "AuraLog.h"

void UAuraGameUtility::GetSpawnLocation(const UObject* WorldContext, const FVector& Center, float Range, int32 SpawnCount, TArray<FVector>& OUTLocations, bool bRandInRange)
{
	const FVector Dir = FVector(1.f, 0.f, 0.f);

	for (int32 i = 1; i < SpawnCount + 1; ++i)
	{
		float RotateDegree = 360.f / SpawnCount * i;
		FVector RotateDir = Dir.RotateAngleAxis(RotateDegree, FVector::UpVector);

		float Distance = bRandInRange ? FMath::RandRange(0.f, Range) : Range;

		FVector SpawnPoint = Center + RotateDir * Distance; 
		SpawnPoint.Z = 0.f;

		OUTLocations.Add(SpawnPoint);
	}
}

TScriptInterface<IStageClearObserver> UAuraGameUtility::GetStageClearObserver(const UObject* WorldContext)
{
	UWorld* World = WorldContext->GetWorld();
	if (!World)
		return nullptr;

	AGameStateBase* GameState = World->GetGameState();
	if (!GameState)
		return nullptr;

	AAuraGameStateBase* AuraGS = Cast<AAuraGameStateBase>(GameState);
	if (!AuraGS)
		return nullptr;

	return AuraGS;
}

UActorFactoryMap* UAuraGameUtility::GetFactoryMap(const UObject* WorldContext)
{
	if (AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext)))
	{
		return AuraGM->GetActorFactoryMap();
	}

	UE_LOG(Aura, Warning, TEXT("UAuraGameUtility::GetFactoryMap() returns Nullptr"));

	return nullptr;
}


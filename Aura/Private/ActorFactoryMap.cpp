// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorFactoryMap.h"
#include "Data/SpawnFactoryInfo.h"
#include "Game/AuraGameInstance.h"
#include "AuraLog.h"


void UActorFactoryMap::RegisterMap()
{
	if (!IsValid(SpawnFactoryInfo))
	{
		SpawnFactoryInfo = LoadObject<USpawnFactoryInfo>(nullptr, TEXT("/Game/Blueprints/Data/DA_SpawnFactoryInfo.DA_SpawnFactoryInfo"));
	}

	check(IsValid(SpawnFactoryInfo));

	for (const auto& Pair : SpawnFactoryInfo.Get()->GetFactoryInfoMap())
	{
		TypeToClassMap.Add(Pair);
	}

	for (const auto& Pair : TypeToClassMap)
	{
		UE_LOG(Aura, Log, TEXT("Key: %d, Value: %s"), Pair.Key, *Pair.Value->GetDefaultObjectName().ToString());
	}
}

void UActorFactoryMap::SpawnAtLocation(UObject* WorldObject, EFactorySpawnActorType Type, const FVector& SpawnLocation, FRotator Rotator)
{
	const TSubclassOf<AActor> SpawnClass = TypeToClassMap[Type];
	if (IsValid(SpawnClass))
	{
		UE_LOG(Aura, Log, TEXT("Key: %d, Value: %s"), Type, *SpawnClass->GetDefaultObjectName().ToString());

		UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldObject);
		check(World);

		FActorSpawnParameters Params;
		World->SpawnActor<AActor>(SpawnClass, SpawnLocation, Rotator, Params);
	}
	else
	{
		UE_LOG(Aura, Log, TEXT("SpawnClass Is NOT Valid!"));
		UE_LOG(Aura, Log, TEXT("TypeToClassMap Num(): %d"), TypeToClassMap.Num());
		for (const auto& Pair : TypeToClassMap)
		{
			UE_LOG(Aura, Log, TEXT("Key: %d, Value: %s"), Pair.Key, *Pair.Value->GetDefaultObjectName().ToString());
		} 
	}
}




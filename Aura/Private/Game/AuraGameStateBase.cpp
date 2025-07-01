// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AuraGameStateBase.h"
#include "AuraLog.h"
#include "Kismet/GameplayStatics.h"
#include "CheckPoint/MapEntrance.h"

void AAuraGameStateBase::SetMapInfo()
{
	CurrentMapName = GetWorld()->GetMapName();
	CurrentMapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	CurrentInfo.Reset();
}

void AAuraGameStateBase::OnTriggerActivated()
{
	CurrentInfo.TriggerCount++;

	if (CheckStageCleared())
		ActivateNextMapEntrance();
}

void AAuraGameStateBase::OnEnemyKilled()
{
	CurrentInfo.EnemyKillCount++;

	if (CheckStageCleared())
		ActivateNextMapEntrance();
}

bool AAuraGameStateBase::CheckStageCleared()
{
	if (!StageClearInfo.Contains(CurrentMapName))
	{
		UE_LOG(Aura, Warning, TEXT("CurrentMapName Or Stage Clear Info Map Is Not Valid"));
		return false;
	}

	const FStageClearInfo& Info = StageClearInfo[CurrentMapName];

	bool IsStageCleared = false;
	if (CurrentInfo.TriggerCount >= Info.TriggerCount && CurrentInfo.EnemyKillCount >= Info.EnemyKillCount)
	{
		IsStageCleared = true;
	}
	
	return IsStageCleared;
}

void AAuraGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	if (!StageClearData)
	{
		UE_LOG(Aura, Warning, TEXT("Stage Clear Data Not Set"));
		return;
	}

	for (const auto& Pair : StageClearData->StageClearInfos)
	{
		StageClearInfo.Add(Pair.Key.ToString(), Pair.Value);
	}
}


void AAuraGameStateBase::ActivateNextMapEntrance()
{
	GetWorld()->GetTimerManager().SetTimer(StageClearedTimer, FTimerDelegate::CreateLambda([this]()
		{
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(this, AMapEntrance::StaticClass(), FoundActors);

			for (AActor* Entrance : FoundActors)
			{
				if (AMapEntrance* MapEntrance = Cast<AMapEntrance>(Entrance))
					MapEntrance->OnStageCleared();
			}

			StageClearedTimer.Invalidate();

		}), StageClearedEventDelay, false);
}

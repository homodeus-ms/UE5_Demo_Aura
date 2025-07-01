// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraBossEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI/AuraAIController.h"
#include "AI/AuraAIDefines.h"
#include "Interface/OpenableInterface.h"
#include "AuraGameUtility.h"
#include "ActorFactoryMap.h"
#include "AuraLog.h"


AAuraBossEnemy::AAuraBossEnemy(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void AAuraBossEnemy::OnComboAttackCompleted_Implementation()
{
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("bBossComboCompleted"), true);
	}
}

void AAuraBossEnemy::Die()
{
	Super::Die();

	GetWorld()->GetTimerManager().SetTimer(SpawnRewardBoxDelayTimer, FTimerDelegate::CreateLambda([this]()
		{
			TArray<FVector> SpawnSpots;
			UAuraGameUtility::GetSpawnLocation(this, GetActorLocation(), 400.f, 5, SpawnSpots);
			UActorFactoryMap* SpawnFactory = UAuraGameUtility::GetFactoryMap(this);
			for (const FVector& Spot : SpawnSpots)
			{
				SpawnFactory->SpawnAtLocation(this, EFactorySpawnActorType::TreasureBox, Spot);
			}

			GetWorld()->GetTimerManager().ClearTimer(SpawnRewardBoxDelayTimer);

		}), SpawnDelayTime, false);

	
	
}

TArray<FVector> AAuraBossEnemy::GetRewardBoxPoints()
{
	TArray<FVector> Points;
	RewardBoxCount = 5;
	const float Radius = 300.f;
	const FVector Center = GetActorLocation();
	const FVector Dir = FVector(1.f, 0.f, 0.f);

	for (int32 i = 1; i < RewardBoxCount + 1; ++i)
	{
		float RotateDegree = 360.f / RewardBoxCount * i;
		FVector RotateDir = Dir.RotateAngleAxis(RotateDegree, FVector::UpVector);
		FVector SpawnPoint = Center + RotateDir * Radius;
		SpawnPoint.Z = 0.f;
		
		Points.Add(SpawnPoint);
	}

	return Points;
}

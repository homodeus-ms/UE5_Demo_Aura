// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA/AuraArcaneSpell.h"
#include "Interface/PlayerInterface.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Actor/PointCollection.h"
#include "AuraLog.h"

void UAuraArcaneSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UAuraArcaneSpell::GetSpawnPoints(int32 InPointNum, float RotationYaw)
{
	if (TargetLocation == FVector::ZeroVector)
	{
		UE_LOG(Aura, Warning, TEXT(
			"Not Valid Target Location,"
			"You Should Receive Valid Target Data From OnTargetDataReady First"));
		return;
	}

	PointCollectActor = GetWorld()->SpawnActor<APointCollection>(PointCollectActorClass, TargetLocation, FRotator());

	PointNum = InPointNum;

	if (PointCollectActor)
	{
		SpawnPoints = PointCollectActor->GetSpawnLocation(TargetLocation, InPointNum, RotationYaw);
	}
}

void UAuraArcaneSpell::CleanUp()
{
	PointCollectActor->Destroy();
	SpawnPoints.Empty();
}

void UAuraArcaneSpell::SpawnInOrder()
{
	check(PointNum > 0);

	int32 Index = 0;

	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimer,
		[this, Index]()
		mutable
		{
			if (Index >= SpawnPoints.Num())
			{
				GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
				OnSpawnFinished.Broadcast();
				return;
			}
			
			OnLocationFixed.Broadcast(SpawnPoints[Index]->GetComponentLocation());

			++Index;
		}
	, 0.2f, true, 0.f);
}


void UAuraArcaneSpell::DrawDebug(USceneComponent* Next)
{
	DrawDebugSphere(GetWorld(), Next->GetComponentLocation(), 30, 20, FColor::Red, false, 10.f);
}



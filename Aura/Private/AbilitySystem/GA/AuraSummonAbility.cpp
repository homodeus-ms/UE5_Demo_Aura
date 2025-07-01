// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA/AuraSummonAbility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AuraLog.h"
#include "AI/AuraAIDefines.h"
#include "Interface/EnemyInterface.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"



void UAuraSummonAbility::ExecutePlayMontageAndWait()
{
	PlayAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		TEXT("PlayAndWait"),
		TaggedMontage.AttackMontage);
	
	PlayAndWaitTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedTask);
	PlayAndWaitTask->OnCancelled.AddDynamic(this, &ThisClass::OnCancelledTask);

	UAbilityTask_WaitGameplayEvent* WaitForTagTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		TaggedMontage.AttackMontageTag);

	WaitForTagTask->EventReceived.AddDynamic(this, &ThisClass::OnReceiveEventTag);

	GetWorld()->GetTimerManager().SetTimer(WaitTimeOver, this, &ThisClass::OnCancelledTask, 3.f, false);

	PlayAndWaitTask->ReadyForActivation();
	WaitForTagTask->ReadyForActivation();
}

void UAuraSummonAbility::SpawnMinions(const TArray<FVector>& SpawnLocations)
{
	check(SpawnLocations.Num() == MaxSpawnCount);

	IEnemyInterface* Shaman = Cast<IEnemyInterface>(GetAvatarActorFromActorInfo());
	check(Shaman);
	
	int32 CurrentMinionCount = Shaman->GetCurrentMinionCount();
	int32 NewMinionCount = MaxSpawnCount - CurrentMinionCount;

	for (int32 i = 0; i < NewMinionCount; ++i)
	{
		const int32 RandomIdx = FMath::RandRange(0, SpawnMinionClasses.Num() - 1);

		FTransform SpawnTransform;

		FVector SpawnLocation = SpawnLocations[i];
		SpawnTransform.SetLocation(SpawnLocation + FVector(0, 0, 100));

		FRotator Rotation(0.f, 180.f, 0.f);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		APawn* Minion = GetWorld()->SpawnActorDeferred<APawn>(
			SpawnMinionClasses[RandomIdx],
			SpawnTransform, 
			nullptr,
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		Minion->SpawnDefaultController();
		Minion->OnDestroyed.AddDynamic(Shaman, &IEnemyInterface::OnMinionDeadCallback);

		Minion->FinishSpawning(SpawnTransform);

		if (IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(Minion))
			EnemyInterface->SetIsSpawnBySummon(true);
	}

	Shaman->AddMinionCount(NewMinionCount);
}

void UAuraSummonAbility::SpawnSpotEffect(const TArray<FVector>& SpawnLocations)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get());

	TArray<FVector_NetQuantize> TempLocations;
	for (const FVector& SpawnLocation : SpawnLocations)
	{
		TempLocations.Add(SpawnLocation);
	}
	
	AuraContext->SetLocation(TempLocations);
	
	FGameplayCueParameters Params;
	Params.EffectCauser = GetAvatarActorFromActorInfo();
	Params.EffectContext = ContextHandle;

	ASC->ExecuteGameplayCue(FAuraGameplayTags::Get().GameplayCue_Summoning, Params);
}


TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	AActor* Instigator = GetAvatarActorFromActorInfo();
	FVector Forward = Instigator->GetActorForwardVector();
	FVector Location = Instigator->GetActorLocation();
	FVector FarLeftSpawnSpot = Forward.RotateAngleAxis(-SpreadAngle / 2.f, FVector::UpVector);
	const float DeltaSpread = SpreadAngle / MaxSpawnCount;

	TArray<FVector> SpawnLocations;

	for (int32 i = 0; i < MaxSpawnCount; ++i)
	{
		const FVector Direction = FarLeftSpawnSpot.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		const float RandomDist = FMath::FRandRange(RandomMinSpawnDist, RandomMaxSpawnDist);
		const FVector SpawnLocation = Location + Direction * RandomDist;
		
		SpawnLocations.Add(SpawnLocation);

	}

	return SpawnLocations;
}



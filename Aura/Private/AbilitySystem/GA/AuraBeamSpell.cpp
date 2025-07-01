// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA/AuraBeamSpell.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Interface/EnemyInterface.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UAuraBeamSpell::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	 
	UE_LOG(Aura, Log, TEXT("UAuraBeamSpell::OnGiveAbility()"));

	if (!OnTargetReadyDelegate.IsAlreadyBound(this, &ThisClass::OnTargetReady))
		OnTargetReadyDelegate.AddDynamic(this, &ThisClass::OnTargetReady);
}

void UAuraBeamSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

bool UAuraBeamSpell::IsTargetEnemyCharacter()
{
	if (IsValid(TargetActor) && TargetActor->Implements<UEnemyInterface>())
		return true;

	return false;
}

void UAuraBeamSpell::OnTargetReady()
{
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetActor))
	{
		FOnDeadDelegate& Delegate = CombatInterface->GetOnDeadDelegate();
		if (!Delegate.IsAlreadyBound(this, &ThisClass::OnFirstTargetEnemyDead))
			Delegate.AddDynamic(this, &ThisClass::OnFirstTargetEnemyDead);
	}
}

TArray<AActor*> UAuraBeamSpell::FindAdditionalEnemiesByLevel()
{
	int32 AbilityLevel = GetAbilityLevel();
	if (AbilityLevel < 2 || TargetActor == nullptr)
		return TArray<AActor*>();

	int32 NumElectrocute = FMath::Min(AbilityLevel, MaxNumElectrocute);

	FVector FirstHitTargetLocation = TargetLocation;
	TArray<AActor*> CloseEnemies;
	TArray<AActor*> IgnoreActors = { TargetActor };
	
	UAuraAbilitySystemLibrary::GetLiveActorsInRange(
		GetAvatarActorFromActorInfo(),
		CloseEnemies, IgnoreActors, FirstHitTargetLocation, ChainEffectActivateRange, EActorSearchType::EnemiesOnly);

	if (CloseEnemies.Num() <= NumElectrocute)
	{
		return CloseEnemies;
	}

	TSet<int32> ChosenIndices;
	TArray<AActor*> Result;
	do
	{
		int32 RandIndex = FMath::RandRange(0, CloseEnemies.Num() - 1);
		if (!ChosenIndices.Contains(RandIndex))
		{
			ChosenIndices.Add(RandIndex);
		}

	} while (ChosenIndices.Num() < NumElectrocute);

	for (int32 Index : ChosenIndices)
	{
		check(CloseEnemies.IsValidIndex(Index));
		Result.Add(CloseEnemies[Index]);

		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(CloseEnemies[Index]))
		{
			FOnDeadDelegate& Delegate = CombatInterface->GetOnDeadDelegate();
			if (!Delegate.IsAlreadyBound(this, &ThisClass::OnAdditionalEnemiesDead))
				Delegate.AddDynamic(this, &ThisClass::OnAdditionalEnemiesDead);
		}
	}

	return Result;
}

void UAuraBeamSpell::ExecutePlayMontageAndWait()
{
	UE_LOG(Aura, Log, TEXT("UAuraBeamSpell::ExecutePlayMontageAndWait()"));

	UAbilityTask_WaitGameplayEvent* WaitForTagTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		TaggedMontage.AttackMontageTag);

	WaitForTagTask->EventReceived.AddDynamic(this, &UAuraTargetedGA::OnReceiveEventTag);

	WaitForTagTask->ReadyForActivation();

	PlayAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		TEXT("PlayAndWait"),
		TaggedMontage.AttackMontage);
	
	PlayAndWaitTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedTask);
	PlayAndWaitTask->OnCancelled.AddDynamic(this, &ThisClass::OnCancelledTask);

	//GetWorld()->GetTimerManager().SetTimer(WaitTimeOver, this, &ThisClass::OnCancelledTask, 20.f, false);

	PlayAndWaitTask->ReadyForActivation();
}


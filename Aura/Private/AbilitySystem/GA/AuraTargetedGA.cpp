 // Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA/AuraTargetedGA.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/AT/TraceTargetDataUnderMouse.h"
#include "Animation/AnimMontage.h"
#include "Interface/CombatInterface.h"
#include "Interface/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AuraLog.h"

void UAuraTargetedGA::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	CombatInterfaceActor = GetAvatarActorFromActorInfo();
	check(CombatInterfaceActor);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraTargetedGA::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (WaitTimeOver.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(WaitTimeOver);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAuraTargetedGA::SetTargetActor()
{
	if (CombatInterfaceActor)
	{
		if (CombatInterfaceActor->IsPlayer())
		{
			UTraceTargetDataUnderMouse* TraceTask = UTraceTargetDataUnderMouse::CreateTask(this);
			TraceTask->OnCursorTargetData.AddDynamic(this, &ThisClass::OnCursorTargetDataCallback);
			TraceTask->ReadyForActivation();
		}
		else
		{
			AActor* EnemyActor = GetAvatarActorFromActorInfo();
			TargetActor = IEnemyInterface::Execute_GetTargetActor(EnemyActor);
			TargetLocation = TargetActor->GetActorLocation();

			OnTargetReadyDelegate.Broadcast();
		}
	}
}

void UAuraTargetedGA::OnCursorTargetDataCallback(const FGameplayAbilityTargetDataHandle DataHandle)
{
	const FGameplayAbilityTargetData* TargetData = DataHandle.Get(0);
	if (!TargetData)
	{
		OnCancelledTask();
		return;
	}

	const FHitResult* Result = TargetData->GetHitResult();
	if (!Result)
	{
		OnCancelledTask();
		return;
	}

	TargetActor = Result->GetActor();
	TargetLocation = Result->ImpactPoint;

	if (!IsTargetInSkillRange())
	{
		OnCancelledTask();
		return;
	}

	OnTargetReadyDelegate.Broadcast();

}

void UAuraTargetedGA::UpdateFacing()
{
	if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
		ICombatInterface::Execute_FacingTarget(GetAvatarActorFromActorInfo(), TargetLocation);
}

void UAuraTargetedGA::PlayMontageAndWaitForRandomTag()
{
	TaggedMontage = CombatInterfaceActor->Execute_GetAttackMontageFromArray(GetAvatarActorFromActorInfo());
	ExecutePlayMontageAndWait();
}

void UAuraTargetedGA::PlayMontageAndWaitForSpecificTag(FGameplayTag MontageTag)
{
	TaggedMontage = CombatInterfaceActor->Execute_GetAttackMontageByMontageTag(GetAvatarActorFromActorInfo(), MontageTag);
	ExecutePlayMontageAndWait();
}

void UAuraTargetedGA::SetCharacterMovementValid(AActor* Actor, bool bValid)
{
	if (ACharacter* Character = Cast<ACharacter>(Actor))
	{
		if (UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
		{
			if (bValid)
			{
				MovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
			}
			else
			{
				MovementComponent->DisableMovement();
			}
		}
	}
}

bool UAuraTargetedGA::IsTargetInSkillRange()
{
	if (!IsValid(TargetActor))
		return false;

	const FVector& ActorLocation = CurrentActorInfo->AvatarActor->GetActorLocation();
	float Distance = (TargetActor->GetActorLocation() - ActorLocation).Length();
	
	return Distance <= SkillRange;
}

void UAuraTargetedGA::ExecutePlayMontageAndWait()
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		UAbilityTask_WaitGameplayEvent* WaitForTagTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			TaggedMontage.AttackMontageTag);

		WaitForTagTask->EventReceived.AddDynamic(this, &ThisClass::OnReceiveEventTag);

		GetWorld()->GetTimerManager().SetTimer(WaitTimeOver, this, &ThisClass::OnWaitTimeEnded, 3.f, false);

		WaitForTagTask->ReadyForActivation();
	}

	PlayAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		TEXT("PlayAndWait"),
		TaggedMontage.AttackMontage);

	
	//PlayAndWaitTask->OnBlendOut.AddDynamic(this, &ThisClass::OnEndTask);
	PlayAndWaitTask->OnCompleted.AddDynamic(this, &ThisClass::OnEndTask);
	PlayAndWaitTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedTask);
	PlayAndWaitTask->OnCancelled.AddDynamic(this, &ThisClass::OnCancelledTask);

	PlayAndWaitTask->ReadyForActivation();
}


void UAuraTargetedGA::OnEndTask()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAuraTargetedGA::OnInterruptedTask()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UAuraTargetedGA::OnCancelledTask()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UAuraTargetedGA::OnWaitTimeEnded()
{
	UE_LOG(Aura, Log, TEXT("Wait For Tag Time Ended()!"));
}
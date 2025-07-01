// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA/AuraMeleeComboAttack.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "AuraGameplayTags.h"


void UAuraMeleeComboAttack::ExecutePlayMontageAndWait()
{
	UAbilityTask_WaitGameplayEvent* WaitForCompleteTagTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FAuraGameplayTags::Get().Boss_Combo_Completed);

	WaitForCompleteTagTask->EventReceived.AddDynamic(this, &ThisClass::OnReceiveComboCompletedEventTag);

	GetWorld()->GetTimerManager().SetTimer(WaitTimeOver, this, &ThisClass::OnCancelledTask, 5.f, false);

	WaitForCompleteTagTask->ReadyForActivation();

	PlayAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		TEXT("PlayAndWait"),
		TaggedMontage.AttackMontage);

	//PlayAndWaitTask->OnBlendOut.AddDynamic(this, &ThisClass::OnEndTask);
	PlayAndWaitTask->OnCompleted.AddDynamic(this, &ThisClass::OnEndTask);
	PlayAndWaitTask->OnInterrupted.AddDynamic(this, &ThisClass::ComboInterrupted);
	PlayAndWaitTask->OnCancelled.AddDynamic(this, &ThisClass::OnCancelledTask);

	UAbilityTask_WaitGameplayEvent* WaitForTagTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		TaggedMontage.AttackMontageTag);

	WaitForTagTask->EventReceived.AddDynamic(this, &ThisClass::OnReceiveEventTag);

	GetWorld()->GetTimerManager().SetTimer(WaitTimeOver, this, &ThisClass::OnCancelledTask, 5.f, false);

	PlayAndWaitTask->ReadyForActivation();
	WaitForTagTask->ReadyForActivation();

}



void UAuraMeleeComboAttack::ComboInterrupted()
{
	OnEndTask();
}




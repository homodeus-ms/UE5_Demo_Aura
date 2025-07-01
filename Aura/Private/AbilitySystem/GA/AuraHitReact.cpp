// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA/AuraHitReact.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"



void UAuraHitReact::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ActiveHandle.IsValid())
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(ActiveHandle);
	}
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, true);
}

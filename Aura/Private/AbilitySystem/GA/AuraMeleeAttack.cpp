// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA/AuraMeleeAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


UAuraMeleeAttack::UAuraMeleeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}


void UAuraMeleeAttack::Attack(AActor* Target)
{
	// Only Server
	bool bServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bServer)
	{
		return;
	}

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		if (CombatInterface->IsTargetInAttackRange(Target))
		{
			ApplyDamage(Target);
		}
		else
		{
			CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		}
	}

}



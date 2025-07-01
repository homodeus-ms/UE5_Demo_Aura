// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA/AuraPassiveGABase.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraLog.h"

UAuraPassiveGABase::UAuraPassiveGABase()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	
}

void UAuraPassiveGABase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		AuraASC->OnPassiveSkillDeactivate.AddUObject(this, &ThisClass::DeActivateAbility);
	}
	if (AbilityTags.Num() > 0)
	{
		FGameplayTag Tag = AbilityTags.GetByIndex(0);
	}
	
}

void UAuraPassiveGABase::DeActivateAbility(const FGameplayTag& AbilityTag)
{
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (AuraASC)
	{
		const FGameplayTag& Tag = AuraASC->GetAbilityTagFromAbilitySpec(*GetCurrentAbilitySpec());
		if (Tag.MatchesTagExact(AbilityTag))
		{
			FString Str = FString::Printf(TEXT("%s Dectivate"), *Tag.ToString());
			CheckValueOnScreen(Str, 1);
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		}
	}
}

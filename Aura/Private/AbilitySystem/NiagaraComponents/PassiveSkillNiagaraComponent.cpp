// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/NiagaraComponents/PassiveSkillNiagaraComponent.h"
#include "Interface/CombatInterface.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraLog.h"
#include "AuraGameplayTags.h"


void UPassiveSkillNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		AuraASC->OnActivatePassiveSkillEffect.AddUObject(this, &ThisClass::ActivateEffectCallback);
		
		ActivateIfAlreadyEquipped(AuraASC);
	}
	else if (CombatInterface)
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
			{
				if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(InASC))
				{
					AuraASC->OnActivatePassiveSkillEffect.AddUObject(this, &ThisClass::ActivateEffectCallback);
					
					ActivateIfAlreadyEquipped(AuraASC);
				}
			});
	}

	if (CombatInterface)
	{
		CombatInterface->GetOnDeadDelegate().AddDynamic(this, &ThisClass::OnActorDead);
	}
}


void UPassiveSkillNiagaraComponent::ActivateEffectCallback(const FGameplayTag& AbilityTag, bool bActivate)
{
	if (AbilityTag.MatchesTagExact(NiagaraTag))
	{
		if (bActivate)
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}

void UPassiveSkillNiagaraComponent::ActivateIfAlreadyEquipped(UAuraAbilitySystemComponent* AuraASC)
{
	const FGameplayTag& StatusTag = AuraASC->GetStatusTagFromAbilityTag(NiagaraTag);
	if (StatusTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		Activate();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GA/AuraGABase.h"
#include "AuraPassiveGABase.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraPassiveGABase : public UAuraGABase
{
	GENERATED_BODY()
	
public:
	UAuraPassiveGABase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void DeActivateAbility(const FGameplayTag& AbilityTag);

protected:
	
};

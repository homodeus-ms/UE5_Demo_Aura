// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GA/AuraGABase.h"
#include "AuraHitReact.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraHitReact : public UAuraGABase
{
	GENERATED_BODY()
	
public:
	//virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FActiveGameplayEffectHandle ActiveHandle;
};

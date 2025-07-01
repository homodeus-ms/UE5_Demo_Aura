// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/NiagaraComponents/AuraNiagaraComponent.h"
#include "PassiveSkillNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UPassiveSkillNiagaraComponent : public UAuraNiagaraComponent
{
	GENERATED_BODY()

public:
	
protected:

	virtual void BeginPlay() override;

	void ActivateEffectCallback(const FGameplayTag& AbilityTag, bool bActivate);


private:
	void ActivateIfAlreadyEquipped(class UAuraAbilitySystemComponent* AuraASC);
};

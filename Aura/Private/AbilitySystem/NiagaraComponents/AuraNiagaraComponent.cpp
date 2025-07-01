// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/NiagaraComponents/AuraNiagaraComponent.h"
#include "Interface/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraLog.h"

UAuraNiagaraComponent::UAuraNiagaraComponent()
{
	bAutoActivate = false;
}

// TObjectPtr<AuraNiagaraComponent> 에서 IsActive를 호출하면 ActorComponent의 IsActive가 호출되는데
// Activate(), Deactivate() 하는 것과는 상관없는 값인 것 같아보임
// 따라서 bSkillActive라는 상태를 따로 관리

void UAuraNiagaraComponent::Activate(bool bReset)
{
	Super::Activate(bReset);

	bSkillActive = true;
}

void UAuraNiagaraComponent::Deactivate()
{
	Super::Deactivate();

	bSkillActive = false;
}

bool UAuraNiagaraComponent::IsSkillActive() const
{
	return bSkillActive;
}

void UAuraNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAuraNiagaraComponent::OnDebuffTagChangedCallback(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UAuraNiagaraComponent::OnActorDead(AActor* DeadActor)
{
	Deactivate();
}

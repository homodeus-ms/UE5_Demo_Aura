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

// TObjectPtr<AuraNiagaraComponent> ���� IsActive�� ȣ���ϸ� ActorComponent�� IsActive�� ȣ��Ǵµ�
// Activate(), Deactivate() �ϴ� �Ͱ��� ������� ���� �� ���ƺ���
// ���� bSkillActive��� ���¸� ���� ����

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

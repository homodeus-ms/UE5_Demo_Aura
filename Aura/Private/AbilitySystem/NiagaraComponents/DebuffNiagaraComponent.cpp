// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/NiagaraComponents/DebuffNiagaraComponent.h"
#include "Interface/CombatInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());

	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ASC->RegisterGameplayTagEvent(NiagaraTag).AddUObject(this, &ThisClass::OnDebuffTagChangedCallback);
	}
	else if (CombatInterface)
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
			{
				InASC->RegisterGameplayTagEvent(NiagaraTag).AddUObject(this, &ThisClass::OnDebuffTagChangedCallback);
			});
	}

	if (CombatInterface)
	{
		CombatInterface->GetOnDeadDelegate().AddDynamic(this, &ThisClass::OnActorDead);
	}
}

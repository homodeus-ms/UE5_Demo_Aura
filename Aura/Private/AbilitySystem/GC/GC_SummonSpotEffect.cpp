// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GC/GC_SummonSpotEffect.h"
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"


bool UGC_SummonSpotEffect::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	FGameplayEffectContextHandle ContextHandle = Parameters.EffectContext;
	FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get());
	
	const TArray<FVector_NetQuantize>& Locations = AuraContext->GetLocations();

	for (const FVector& Location : Locations)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			MyTarget,
			SpotEffect,
			Location
		);
	}

	return true;
}

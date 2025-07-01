// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UExecCalc_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	float GetAttributeValue(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectAttributeCaptureDefinition& CaptureDef, const FAggregatorEvaluateParameters& EvaluateParam) const;

	float CalculateDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
		const FAggregatorEvaluateParameters& EvaluateParams, 
		const FGameplayEffectSpec& EffectSpec,
		bool& bBlocked, bool& bCritHit) const;

	bool CalculateDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		const FAggregatorEvaluateParameters& EvaluateParams,
		const FGameplayEffectSpec& EffectSpec, FGameplayTag& OUTDamageTypeTag) const;

	FGameplayTag GetResistanceTagByDamageTypeTag(const FGameplayTag& DamageTypeTag) const;

	float GetTargetResistanceValue(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParam, const FGameplayTag& ResistanceTag) const;


private:

};



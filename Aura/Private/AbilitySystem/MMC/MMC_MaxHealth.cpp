// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_MaxHealth.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interface/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorCaptureDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorCaptureDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorCaptureDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorCaptureDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorCaptureDef, Spec, EvaluateParams, Vigor);
	Vigor = FMath::Max<float>(0.f, Vigor);

	ICombatInterface* Character = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	uint8 CharacterLevel = Character->GetCharacterLevel();

	return Vigor * 2.5f + 80.f + CharacterLevel * 10.f;
}

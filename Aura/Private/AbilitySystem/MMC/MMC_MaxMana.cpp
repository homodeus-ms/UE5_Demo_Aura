// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_MaxMana.h"
#include "Interface/CombatInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"

UMMC_MaxMana::UMMC_MaxMana()
{
    IntelligenceCaptureDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
    IntelligenceCaptureDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    IntelligenceCaptureDef.bSnapshot = false;

    RelevantAttributesToCapture.Add(IntelligenceCaptureDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluateParams;
    EvaluateParams.SourceTags = SourceTags;
    EvaluateParams.TargetTags = TargetTags;

    float Intelligence = 0.f;
    GetCapturedAttributeMagnitude(IntelligenceCaptureDef, Spec, EvaluateParams, Intelligence);
    Intelligence = FMath::Max<float>(0.f, Intelligence);

    ICombatInterface* Character = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
    uint8 CharacterLevel = Character->GetCharacterLevel();

    return Intelligence * 2.5f + 50.f + CharacterLevel * 15.f;
}

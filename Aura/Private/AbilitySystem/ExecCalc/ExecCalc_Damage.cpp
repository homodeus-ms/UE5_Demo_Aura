// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameplayEffectAggregator.h"
#include "AuraGameplayTags.h"
#include "AuraLog.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interface/PlayerInterface.h"
#include "AuraAbilityTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

/* Note : Macros Defines In Engine
* 
	#define DECLARE_ATTRIBUTE_CAPTUREDEF(P) \
		FProperty* P##Property; \
		FGameplayEffectAttributeCaptureDefinition P##Def; \

	#define DEFINE_ATTRIBUTE_CAPTUREDEF(S, P, T, B) \
	{ \
		P##Property = FindFieldChecked<FProperty>(S::StaticClass(), GET_MEMBER_NAME_CHECKED(S, P)); \
		P##Def = FGameplayEffectAttributeCaptureDefinition(P##Property, EGameplayEffectAttributeCaptureSource::T, B); \
	}
*/

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagToCaptureDef;

	AuraDamageStatics()
	{
		// ( ClassName, PropertyName, Source/Target, Snapshop )
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);

		const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();

		/* FAuraGameplayTag가 Init 되기전에 밑의 코드들이 실행되면 문제가 생김!
		*  static AuraDamageStatics DStatics; 코드에 의해서 
		*  이 생성자로 들어오는 것이 FAuraGameplayTag가 Init 되는 것보다 더 빠름 */
		
		if (!Tags.IsLoaded())
		{
			Tags.InitAuraGameplayTags();
		}

		TagToCaptureDef.Add(Tags.Attribute_Secondary_Armor, ArmorDef);
		TagToCaptureDef.Add(Tags.Attribute_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagToCaptureDef.Add(Tags.Attribute_Secondary_BlockChance, BlockChanceDef);
		TagToCaptureDef.Add(Tags.Attribute_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagToCaptureDef.Add(Tags.Attribute_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagToCaptureDef.Add(Tags.Attribute_Secondary_CriticalHitResistance, CriticalHitResistanceDef);

		TagToCaptureDef.Add(Tags.Attribute_Resistance_Fire, FireResistanceDef);
		TagToCaptureDef.Add(Tags.Attribute_Resistance_Lightning, LightningResistanceDef);
		TagToCaptureDef.Add(Tags.Attribute_Resistance_Arcane, ArcaneResistanceDef);
		TagToCaptureDef.Add(Tags.Attribute_Resistance_Physical, PhysicalResistanceDef);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatarActor = SourceASC->GetAvatarActor();
	const AActor* TargetAvatarActor = TargetASC->GetAvatarActor();

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	bool bBlocked = false;
	bool bCritHit = false;
	float Damage = CalculateDamage(ExecutionParams, EvaluateParams, EffectSpec, bBlocked, bCritHit);
	FGameplayTag DamageTypeTag = FGameplayTag();
	bool bDebuff = CalculateDebuff(ExecutionParams, EvaluateParams, EffectSpec, DamageTypeTag);

	// Send Blocked, CritHit Info Using Custom GEContext
	FGameplayEffectContextHandle ContextHandle = EffectSpec.GetContext();
	UAuraAbilitySystemLibrary::SetIsBlocked(ContextHandle, bBlocked);
	UAuraAbilitySystemLibrary::SetIsCirticalHit(ContextHandle, bCritHit);

	if (bDebuff)
	{
		check(DamageTypeTag.IsValid());

		const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

		const float DebuffDamage = EffectSpec.GetSetByCallerMagnitude(AuraTags.Debuff_Damage, false, -1.f);
		const float DebuffDuration = EffectSpec.GetSetByCallerMagnitude(AuraTags.Debuff_Duration, false, -1.f);
		const float DebuffFrequency = EffectSpec.GetSetByCallerMagnitude(AuraTags.Debuff_Frequency, false, -1.f);

		UAuraAbilitySystemLibrary::SetDebuffActivate(ContextHandle, bDebuff);
		UAuraAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
		UAuraAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
		UAuraAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
		UAuraAbilitySystemLibrary::SetDamageTypeTag(ContextHandle, DamageTypeTag);
	}


	const FGameplayModifierEvaluatedData EvaluatedData(
		UAuraAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, Damage);

	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

float UExecCalc_Damage::CalculateDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, const FGameplayEffectSpec& EffectSpec, bool& bBlocked, bool& bCritHit) const
{
	FGameplayEffectContextHandle ContextHandle = EffectSpec.GetContext();
	AActor* Target = ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor();
	check(Target != nullptr);

	float TargetBlock = GetAttributeValue(ExecutionParams, DamageStatics().BlockChanceDef, EvaluateParams);
	float TargetArmor = GetAttributeValue(ExecutionParams, DamageStatics().ArmorDef, EvaluateParams);
	float SourceArmorPenetration = GetAttributeValue(ExecutionParams, DamageStatics().ArmorPenetrationDef, EvaluateParams);
	float CritHitChance = GetAttributeValue(ExecutionParams, DamageStatics().CriticalHitChanceDef, EvaluateParams);
	float CritHitDamage = GetAttributeValue(ExecutionParams, DamageStatics().CriticalHitDamageDef, EvaluateParams);
	float CritHitResistance = GetAttributeValue(ExecutionParams, DamageStatics().CriticalHitResistanceDef, EvaluateParams);

	const TMap<FGameplayTag, FGameplayTag>& DamageResistanceMatchMap = FAuraGameplayTags::Get().DamageTypeToResistance;
	float Damage = 0.f;

	for (const auto& Pair : DamageResistanceMatchMap)
	{
		const FGameplayTag DamageTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		// GetSetByCallerMagnitude에 값이 없었으면 0을 반환함
		float DamageValue = EffectSpec.GetSetByCallerMagnitude(DamageTag, false);
		check(DamageStatics().TagToCaptureDef.Contains(ResistanceTag));
		float TargetResistance = GetTargetResistanceValue(ExecutionParams, EvaluateParams, ResistanceTag);

		DamageValue *= (100 - TargetResistance) / 100.f;

		Damage += DamageValue;
	}

	// Radial Damage
	if (UAuraAbilitySystemLibrary::IsRadialDamage(ContextHandle))
	{
		FRadialDamageParams Params;
		Params.BaseDamage = Damage;
		Params.InnerRadius = UAuraAbilitySystemLibrary::GetRadialInnerRadius(ContextHandle);
		Params.OuterRadius = UAuraAbilitySystemLibrary::GetRadialOuterRadius(ContextHandle);
		Params.MinimumDamage = 0.f;
		Params.DamageFalloff = 1.f;

		float Distance = (UAuraAbilitySystemLibrary::GetRadialDamageOrigin(ContextHandle) - Target->GetActorLocation()).Length();
		float DamageScale = Params.GetDamageScale(Distance);

		Damage *= DamageScale;
	}

	bBlocked = FMath::RandRange(1, 50) < TargetBlock;
	TargetArmor *= FMath::Max(0, (100 - SourceArmorPenetration) / 100.f);
	Damage *= FMath::Max(0, (100 - TargetArmor) / 100.f);
	CritHitChance *= FMath::Max(0, (100 - CritHitResistance) / 100.f);

	bCritHit = !bBlocked && FMath::RandRange(1, 50) < CritHitChance;

	if (bBlocked)
	{
		Damage *= 0.5f;
	}
	else if (bCritHit)
	{
		Damage += CritHitDamage * 2.f;
	}

	// If Target Is Player, Check HaloOfProtection Passive Skill
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(Target))
	{
		if (PlayerInterface->IsHaloOfProtectionActive())
		{
			Damage *= 0.5f;
		}
	}

	Damage = Damage == 0 ? 1 : Damage;

	return Damage;
}

bool UExecCalc_Damage::CalculateDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, const FGameplayEffectSpec& EffectSpec, FGameplayTag& OUTDamageTypeTag) const
{
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	bool bDebuffActivate = false;

	const TMap<FGameplayTag, FGameplayTag>& DamageTypeToDebuff = Tags.DamageTypeToDebuff;

	for (const auto& Pair : DamageTypeToDebuff)
	{
		const FGameplayTag& DamageTypeTag = Pair.Key;
		const FGameplayTag& DebuffTag = Pair.Value;
		const float TypeDamage = EffectSpec.GetSetByCallerMagnitude(DamageTypeTag, false, -1.f);

		if (TypeDamage > 0.f)
		{
			const FGameplayTag& ResistanceTag = GetResistanceTagByDamageTypeTag(DamageTypeTag);
			check(DamageStatics().TagToCaptureDef.Contains(ResistanceTag));
			const float TargetResistanceValue = GetTargetResistanceValue(ExecutionParams, EvaluateParams, ResistanceTag);

			const float SourceDebuffChance = EffectSpec.GetSetByCallerMagnitude(Tags.Debuff_Chance, false, -1.f);
			if (SourceDebuffChance > 0.f)
			{
				float DebuffChance = SourceDebuffChance * (100.f - TargetResistanceValue) / 100.f;
				bDebuffActivate = FMath::RandRange(1, 100) < DebuffChance;
			}

			OUTDamageTypeTag = DamageTypeTag;

			break;
		}
	}

	return bDebuffActivate;
}

FGameplayTag UExecCalc_Damage::GetResistanceTagByDamageTypeTag(const FGameplayTag& DamageTypeTag) const
{
	check(FAuraGameplayTags::Get().DamageTypeToResistance.Contains(DamageTypeTag));
	return FAuraGameplayTags::Get().DamageTypeToResistance[DamageTypeTag];
}

float UExecCalc_Damage::GetAttributeValue(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectAttributeCaptureDefinition& CaptureDef, const FAggregatorEvaluateParameters& EvaluateParam) const
{
	float Value;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		CaptureDef, EvaluateParam, Value);

	return FMath::Max<float>(0.f, Value);
}

float UExecCalc_Damage::GetTargetResistanceValue(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParam, const FGameplayTag& ResistanceTag) const
{
	const FGameplayEffectAttributeCaptureDefinition CaptureDef = DamageStatics().TagToCaptureDef[ResistanceTag];

	return GetAttributeValue(ExecutionParams, CaptureDef, EvaluateParam);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA/AuraDamageGiveGA.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraLog.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "GameplayEffect.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UAuraDamageGiveGA::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraDamageGiveGA::ApplyDamage(AActor* InTargetActor)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InTargetActor);
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	
	const FDamageEffectParams& Params = MakeDamageEffectParams(InTargetActor);
	FGameplayEffectContextHandle ContextHandle =  UAuraAbilitySystemLibrary::ApplyDamageEffect(Params);

	if (ContextHandle.IsValid())
	{
		bHitTarget = true;
	}
	
}

void UAuraDamageGiveGA::SetByCallerMagnitude(const FGameplayEffectSpecHandle& SpecHandle)
{
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(DamageTypeTag, DamageBase.GetValueAtLevel(GetAbilityLevel()));
}

float UAuraDamageGiveGA::GetDamageByLevel(int32 Level) const
{
	return DamageBase.GetValueAtLevel(Level);
}

float UAuraDamageGiveGA::GetManaCost(int32 Level) const
{
	float ManaCost = 0.f;

	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (const FGameplayModifierInfo& ModInfo : CostEffect->Modifiers)
		{
			if (ModInfo.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				ModInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, ManaCost);
				break;
			}
		}
	}

	return ManaCost;
}

float UAuraDamageGiveGA::GetCooltime(int32 Level) const
{
	float Cooltime = 0.f;
	if (UGameplayEffect* CooltimeEffect = GetCooldownGameplayEffect())
	{
		CooltimeEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(Level, Cooltime);
	}
	return Cooltime;
}

FDamageEffectParams UAuraDamageGiveGA::MakeDamageEffectParams(AActor* TargetActorOrNull) const
{
	FDamageEffectParams Params;
	Params.WorldContext = GetAvatarActorFromActorInfo();
	Params.DamageEffectClass = DamageEffectClass;
	Params.SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());

	// If TargetActor Is Null, 'GetAbilitySystemComponent' Return Nullptr
	Params.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActorOrNull);

	Params.BaseDamage = DamageBase.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageTypeTag = DamageTypeTag;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffFrequency = DebuffFrequency;
	
	Params.KnockbackForce = KnockbackForce;
	Params.KnockbackChance = KnockbackChance;

	if (bIsRadialDamage)
	{
		Params.bIsRadialDamage = bIsRadialDamage;
		Params.RadialInnerRadius = RadialInnerRadius;
		Params.RadialOuterRadius = RadialOuterRadius;
		Params.RadialDamageOrigin = RadialDamageOrigin;
	}

	return Params;
}

FString UAuraDamageGiveGA::GetDescription(const FString& SkillName, int32 Level)
{
	const float Damage = GetDamageByLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooltime = GetCooltime(Level);

	return FString::Printf(TEXT(
		"<Title>%s</>\n"
		"<Middle>(Lv. </><Level>%d</><Middle>)</>\n\n"
		"데미지 - <Damage>%0.1f</>\n"
		"마나소비 - <Cost>%0.1f</>\n"
		"쿨타임 - <Cooltime>%0.1f</>\n\n"
		"타겟에게 <Damage>%0.1f</>의 데미지를 줍니다\n"),

		*SkillName, Level, Damage, ManaCost, Cooltime, Damage);
}

FString UAuraDamageGiveGA::GetNextLevelDescription(const FString& SkillName, int32 Level)
{
	Level++;
	const float Damage = GetDamageByLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooltime = GetCooltime(Level);

	return FString::Printf(TEXT(
		"<Title>%s</>\n"
		"<Middle>(Lv. </><Level>%d</><Middle>)</>\n\n"
		"데미지 - <Damage>%0.1f</>\n"
		"마나소비 - <Cost>%0.1f</>\n"
		"쿨타임 - <Cooltime>%0.1f</>\n\n"
		"타겟에게 <Damage>%0.1f</>의 데미지를 줍니다\n"),

		*SkillName, Level, Damage, ManaCost, Cooltime, Damage);
}

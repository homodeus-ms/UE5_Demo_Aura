// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GA/AuraTargetedGA.h"
#include "AuraAbilityTypes.h"
#include "AuraDamageGiveGA.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGiveGA : public UAuraTargetedGA
{
	GENERATED_BODY()
	
public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	virtual void ApplyDamage(AActor* Target);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool HasHitTarget() const { return bHitTarget; }

	UFUNCTION(BlueprintCallable)
	float GetDamageByLevel(int32 Level) const;
	float GetManaCost(int32 Level) const;
	float GetCooltime(int32 Level) const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TSubclassOf<class UGameplayEffect> GetDamageEffectClass() const { return DamageEffectClass; }

	UFUNCTION(BlueprintCallable)
	FDamageEffectParams MakeDamageEffectParams(AActor* TargetActorOrNull = nullptr) const;

	virtual FString GetDescription(const FString& SkillName, int32 Level = 1) override;
	virtual FString GetNextLevelDescription(const FString& SkillName, int32 Level = 1) override;

protected:

	void SetByCallerMagnitude(const FGameplayEffectSpecHandle& SpecHandle);

	UPROPERTY(EditAnywhere, Category = "Damage|Effect")
	TSubclassOf<class UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditAnywhere, Category = "Damage|Effect")
	FScalableFloat DamageBase = 0.f;

	UPROPERTY(EditAnywhere, Category = "Damage|Effect")
	FGameplayTag DamageTypeTag = FGameplayTag();

	UPROPERTY(EditAnywhere, Category = "Debuff")
	float DebuffChance = 20.f;

	UPROPERTY(EditAnywhere, Category = "Debuff")
	float DebuffDamage = 2.f;

	UPROPERTY(EditAnywhere, Category = "Debuff")
	float DebuffDuration = 5.f;

	UPROPERTY(EditAnywhere, Category = "Debuff")
	float DebuffFrequency = 1.f;

	// Knockback
	UPROPERTY(EditAnywhere, Category = "Knockback")
	float KnockbackForce = 300.f;

	UPROPERTY(EditAnywhere, Category = "Knockback")
	float KnockbackChance = 20.f;

	// Radial Damage

	UPROPERTY(EditDefaultsOnly, Category="RadialDamage")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RadialDamage")
	float RadialInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RadialDamage")
	float RadialOuterRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;
	
private:

	bool bHitTarget = false;
};

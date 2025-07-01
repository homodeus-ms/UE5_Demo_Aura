#pragma once

#include "GameplayEffectTypes.h"
#include "AuraLog.h"
#include "AuraAbilityTypes.generated.h"


USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {}

	UPROPERTY()
	TObjectPtr<UObject> WorldContext = nullptr;

	UPROPERTY()
	TSubclassOf<class UGameplayEffect> DamageEffectClass = nullptr;

	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> SourceASC = nullptr;

	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> TargetASC = nullptr;

	UPROPERTY()
	float BaseDamage = 0.f;

	UPROPERTY()
	float AbilityLevel = 1.f;

	UPROPERTY()
	FGameplayTag DamageTypeTag = FGameplayTag();

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffChance = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	// Knockback
	UPROPERTY()
	float KnockbackForce = 0.f;

	UPROPERTY()
	float KnockbackChance = 0.f;

	UPROPERTY()
	FVector KnockbackPos = FVector::Zero();

	// Radial Damage
	UPROPERTY()
	bool bIsRadialDamage = false;

	UPROPERTY()
	float RadialInnerRadius = 0.f;

	UPROPERTY()
	float RadialOuterRadius = 0.f;

	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
};


USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	// Block, CritHit
	FORCEINLINE bool IsBlocked() const { return bBlocked; }
	FORCEINLINE bool IsCriticalHit() const { return bCritHit; }
	FORCEINLINE void SetIsBlocked(bool InbBlocked) { bBlocked = InbBlocked; }
	FORCEINLINE void SetIsCriticalHit(bool InbCriticalHit) { bCritHit = InbCriticalHit; }

	// Debuff
	FORCEINLINE bool ShouldDebuffActivate() const { return bDebuffActivate; }
	FORCEINLINE float GetDebuffDamage() const { return DebuffDamage; }
	FORCEINLINE float GetDebuffDuration() const { return DebuffDuration; }
	FORCEINLINE float GetDebuffFrequency() const { return DebuffFrequency; }
	FORCEINLINE TWeakPtr<FGameplayTag> GetDebuffDamageTypeTag() const { return DamageTypeTag; }

	FORCEINLINE void SetDebuffActivate(bool bActivate) { bDebuffActivate = bActivate; }
	FORCEINLINE void SetDebuffDamage(float InDebuffDamage) { DebuffDamage = InDebuffDamage; }
	FORCEINLINE void SetDebuffDuration(float InDebuffDuration) { DebuffDuration = InDebuffDuration; }
	FORCEINLINE void SetDebuffFrequency(float InDebuffFrequency) { DebuffFrequency = InDebuffFrequency; }
	FORCEINLINE void SetDamgeTypeTag(const TSharedPtr<FGameplayTag>& TypeTag) { DamageTypeTag = TypeTag; }

	// Knockback
	FORCEINLINE FVector GetKnockbackPos() const { return KnockbackPos; }
	FORCEINLINE void SetKnockbackPos(FVector InPos) { KnockbackPos = InPos; }

	// Radial Damage
	FORCEINLINE bool IsRadialDamage() const { return bIsRadialDamage; }
	FORCEINLINE float GetRadialInnerRadius() const { return RadialInnerRadius; }
	FORCEINLINE float GetRadialOuterRadius() const { return RadialOuterRadius; }
	FORCEINLINE FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }

	FORCEINLINE void SetIsRadialDamage(bool InIsRadialDamage) { bIsRadialDamage = InIsRadialDamage; }
	FORCEINLINE void SetRadialInnerRadius(float InRadius) { RadialInnerRadius = InRadius; }
	FORCEINLINE void SetRadialOuterRadius(float InRadius) { RadialOuterRadius = InRadius; }
	FORCEINLINE void SetRadialDamageOrigin(const FVector& Origin) { RadialDamageOrigin = Origin; }
	
	void SetLocation(const TArray<FVector_NetQuantize>& Locations);
	const TArray<FVector_NetQuantize>& GetLocations() const;
	
	virtual FAuraGameplayEffectContext* Duplicate() const override
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:
	UPROPERTY()
	bool bBlocked = false;

	UPROPERTY()
	bool bCritHit = false;

	UPROPERTY()
	bool bDebuffActivate = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;
	
	TSharedPtr<struct FGameplayTag> DamageTypeTag;

	UPROPERTY()
	FVector KnockbackPos = FVector();

	UPROPERTY()
	bool bIsRadialDamage = false;

	UPROPERTY()
	float RadialInnerRadius = 0.f;

	UPROPERTY()
	float RadialOuterRadius = 0.f;

	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;

	// Max Count is 10
	UPROPERTY()
	TArray<FVector_NetQuantize> SpawnLocations;
};

template<>
struct TStructOpsTypeTraits< FAuraGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FAuraGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
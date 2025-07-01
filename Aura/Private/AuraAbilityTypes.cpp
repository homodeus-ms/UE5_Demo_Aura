#include "AuraAbilityTypes.h"

void FAuraGameplayEffectContext::SetLocation(const TArray<FVector_NetQuantize>& Locations)
{
	SpawnLocations.Reset();
	SpawnLocations.Append(Locations);
}

const TArray<FVector_NetQuantize>& FAuraGameplayEffectContext::GetLocations() const
{
	return SpawnLocations;
}

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{

	uint32 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid())
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (bBlocked)
		{
			RepBits |= 1 << 7;
		}
		if (bCritHit)
		{
			RepBits |= 1 << 8;
		}
		if (SpawnLocations.Num() > 0)
		{
			RepBits |= 1 << 9;
		}
		if (bDebuffActivate)
		{
			RepBits |= 1 << 10;    // bDebuff Activate
			RepBits |= 1 << 11;    // Debuff Damage
			RepBits |= 1 << 12;    // Debuff Duration
			RepBits |= 1 << 13;    // Debuff Frequency
			RepBits |= 1 << 14;    // Debuff DamageTypeTag
		}
		if (KnockbackPos != FVector::ZeroVector)
		{
			RepBits |= 1 << 15;
		}
		if (bIsRadialDamage)
		{
			RepBits |= 1 << 16;    // bIsRadialDamage;
			if (RadialInnerRadius != 0.f)
				RepBits |= 1 << 17;    // RadialInnerRadius;
			if (RadialOuterRadius != 0.f)
				RepBits |= 1 << 18;    // RadialOuterRadius;
			RepBits |= 1 << 19;    // RadialDamageOrigin;
		}
	}

	Ar.SerializeBits(&RepBits, 20);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	if (RepBits & (1 << 7))
	{
		Ar << bBlocked;
	}
	if (RepBits & (1 << 8))
	{
		Ar << bCritHit;
	}
	if (RepBits & (1 << 9))
	{
		SafeNetSerializeTArray_WithNetSerialize<5, FVector_NetQuantize>(Ar, SpawnLocations, Map);
	}
	if (RepBits & (1 << 10))
	{
		Ar << bDebuffActivate;
	}
	if (RepBits & (1 << 11))
	{
		Ar << DebuffDamage;
	}
	if (RepBits & (1 << 12))
	{
		Ar << DebuffDuration;
	}
	if (RepBits & (1 << 13))
	{
		Ar << DebuffFrequency;
	}
	if (RepBits & (1 << 14))
	{
		if (Ar.IsLoading())
		{
			if (!DamageTypeTag.IsValid())
			{
				DamageTypeTag = TSharedPtr<FGameplayTag>(new FGameplayTag());
			}
		}
		DamageTypeTag->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 15))
	{
		Ar << KnockbackPos;
	}
	if (RepBits & (1 << 16))
	{
		Ar << bIsRadialDamage;
		Ar << RadialInnerRadius;
		Ar << RadialOuterRadius;
		RadialDamageOrigin.NetSerialize(Ar, Map, bOutSuccess);
	}

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}

	bOutSuccess = true;

	return true;
}

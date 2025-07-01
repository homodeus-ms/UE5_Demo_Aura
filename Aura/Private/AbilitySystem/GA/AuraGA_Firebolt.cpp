// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA/AuraGA_Firebolt.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "AuraLog.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SceneComponent.h"

UAuraGA_Firebolt::UAuraGA_Firebolt()
{
}

FString UAuraGA_Firebolt::GetDescription(const FString& SkillName, int32 Level)
{
	const float Damage = GetDamageByLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooltime = GetCooltime(Level);
	const int32 NumProjectiles = FMath::Min(Level, MaxNumOfProjectile);

	return FString::Printf(TEXT(
		"<Title>%s</>\n"
		"<Middle>(Lv. </><Level>%d</><Middle>)</>\n\n"
		"데미지 - <Damage>%0.1f</>\n"
		"마나소비 - <Cost>%0.1f</>\n"
		"쿨타임 - <Cooltime>%0.1f</>\n\n"
		"타겟에게 <Level>%d</>개의 일반 파이어볼을 발사.\n각 <Damage>%0.1f</>의 데미지를 줍니다.\n\n"
		"<DebuffTitle>화염 디버프</>\n"
		"디버프 발생 확률 : <Chance>%0.f%%</>\n"
		"화염 디버프가 발생 하면 일정 시간동안 지속적으로 적의 체력을 깎습니다.\n"
	
	),

		*SkillName, Level, Damage, ManaCost, Cooltime, NumProjectiles, Damage, DebuffChance);
}

FString UAuraGA_Firebolt::GetNextLevelDescription(const FString& SkillName, int32 Level)
{
	Level++;
	const float Damage = GetDamageByLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooltime = GetCooltime(Level);
	const int32 NumProjectiles = FMath::Min(Level, MaxNumOfProjectile);


	return FString::Printf(TEXT(
		"<Title>%s</>\n"
		"<Middle>(Lv. </><Level>%d</><Middle>)</>\n\n"
		"데미지 - <Damage>%0.1f</>\n"
		"마나소비 - <Cost>%0.1f</>\n"
		"쿨타임 - <Cooltime>%0.1f</>\n\n"
		"타겟에게 <Level>%d</>개의 유도 파이어볼을 발사.\n각 <Damage>%0.1f</>의 데미지를 줍니다.\n\n"
		"<DebuffTitle>화염 디버프</>\n"
		"디버프 발생 확률 : <Chance>%0.f%%</>\n"
		"화염 디버프가 발생 하면 일정 시간동안 지속적으로 적의 체력을 깎습니다.\n"
	),

		*SkillName, Level, Damage, ManaCost, Cooltime, NumProjectiles, Damage, DebuffChance);
}

void UAuraGA_Firebolt::SpawnProjectiles(const FVector& InTargetLocation, const FGameplayTag& CombatSocketTag)
{
	bool bServer = GetAvatarActorFromActorInfo()->HasAuthority();
	
	// Only Server
	if (!bServer || !ProjectileClass)
	{
		return;
	}

	if (GetAbilityLevel() == 1)
	{
		SpawnProjectile(InTargetLocation, CombatSocketTag);
		return;
	}

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const FVector Forward = AvatarActor->GetActorForwardVector();
	int32 NumOfFirebolt = FMath::Min(GetAbilityLevel(), MaxNumOfProjectile);
	const TArray<FVector> SpreadVectors = UAuraAbilitySystemLibrary::GetVectorsSpread(Forward, FVector::UpVector, SpreadAngle, NumOfFirebolt);
	const TArray<FRotator> SpreadRotatos = UAuraAbilitySystemLibrary::GetRotatorsSpread(Forward, FVector::UpVector, SpreadAngle, NumOfFirebolt);

	ICombatInterface* CombatActor = Cast<ICombatInterface>(AvatarActor);
	if (CombatActor)
	{
		const FVector CombatSocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(AvatarActor, CombatSocketTag);
		
		for (const FRotator& Rot : SpreadRotatos)
		{
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(CombatSocketLocation);
			SpawnTransform.SetRotation(Rot.Quaternion());

			AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
				ProjectileClass,
				SpawnTransform,
				GetOwningActorFromActorInfo(),
				Cast<APawn>(GetOwningActorFromActorInfo()),
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			Projectile->DamageEffectParams = MakeDamageEffectParams();

			// Homing 
			if (ICombatInterface* CombatTarget = Cast<ICombatInterface>(TargetActor))
			{
				UE_LOG(Aura, Log, TEXT("TargetActor Implements CombatInterface!"));
				Projectile->SetHomingTarget(TargetActor);
			}
			else
			{
				UE_LOG(Aura, Log, TEXT("TargetActor NOT Implements CombatInterface!"));

				Projectile->CachedHomingProjectileComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
				Projectile->CachedHomingProjectileComponent->SetWorldLocation(TargetLocation);
				Projectile->ProjectileMovement->HomingTargetComponent = Projectile->CachedHomingProjectileComponent;

			}
			Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::RandRange(MinAcceleration, MaxAcceleration);
			Projectile->ProjectileMovement->bIsHomingProjectile = bHomingProjectile;

			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}

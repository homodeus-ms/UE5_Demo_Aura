// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA/AuraSpawnProjectile.h"
#include "Actor/AuraProjectile.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AuraLog.h"

UAuraSpawnProjectile::UAuraSpawnProjectile()
{
	
}

void UAuraSpawnProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraSpawnProjectile::SpawnProjectile(const FVector& InTargetLocation, const FGameplayTag& CombatSocketTag)
{
	// Only Server
	bool bServer = GetAvatarActorFromActorInfo()->HasAuthority();

	if (!bServer || !ProjectileClass)
	{
		return;
	}

	AActor* AvatarActor = GetAvatarActorFromActorInfo();

	ICombatInterface* CombatActor = Cast<ICombatInterface>(AvatarActor);
	if (CombatActor)
	{
		const FVector CombatSocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(AvatarActor, CombatSocketTag);
		FRotator SpawnRotation = (InTargetLocation - CombatSocketLocation).Rotation();
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(CombatSocketLocation);
		SpawnTransform.SetRotation(SpawnRotation.Quaternion());

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// Make Effect Params
		Projectile->DamageEffectParams = MakeDamageEffectParams();
		
		// Finish Spawning
		Projectile->FinishSpawning(SpawnTransform);
	}
}

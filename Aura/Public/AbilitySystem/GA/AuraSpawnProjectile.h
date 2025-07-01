// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GA/AuraDamageGiveGA.h"
#include "AuraSpawnProjectile.generated.h"


class UGameplayEffect;
/**
 * 
 */
UCLASS()
class AURA_API UAuraSpawnProjectile : public UAuraDamageGiveGA
{
	GENERATED_BODY()
	
public:
	UAuraSpawnProjectile();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FVector& InTargetLocation, const FGameplayTag& CombatSocketTag);

	UPROPERTY(EditAnywhere, Category="Projectile")
	TSubclassOf<class AAuraProjectile> ProjectileClass;

	// Temp
	UPROPERTY(EditAnyWhere, Category="AbilityLevel")
	float AbilityLevel = 1.f;
};

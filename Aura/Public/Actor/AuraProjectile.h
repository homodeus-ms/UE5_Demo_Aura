// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.h"
#include "AuraProjectile.generated.h"

class USoundBase;
class UNiagaraSystem;
class UGameplayEffect;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AAuraProjectile();

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetHomingTarget(AActor* Target);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY()
	TObjectPtr<class USceneComponent> CachedHomingProjectileComponent;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	void OnHit();

	UFUNCTION()
	void OnTargetEnemyDead(AActor* DeadActor);

private:
	UPROPERTY(EditDefaultsOnly, category="Effect")
	TSubclassOf<UGameplayEffect> AppliedEffectClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> SphereComponent;

	UPROPERTY(EditAnywhere, category="Effect")
	TObjectPtr<UNiagaraSystem> HitEffect;

	UPROPERTY(EditAnywhere, category = "Effect")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, category = "Effect")
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY()
	TObjectPtr<class UAudioComponent> LoopingSoundComponent;
	
	bool bClientAlreadyHandledOnHit = false;

	UPROPERTY(EditAnywhere)
	float LifeSpanTime = 5.f;
};

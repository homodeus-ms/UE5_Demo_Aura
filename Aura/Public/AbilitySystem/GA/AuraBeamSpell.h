// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GA/AuraDamageGiveGA.h"
#include "AuraBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraBeamSpell : public UAuraDamageGiveGA
{
	GENERATED_BODY()
	
public:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	bool IsTargetEnemyCharacter();

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> FindAdditionalEnemiesByLevel();

	UFUNCTION()
	void OnTargetReady();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnFirstTargetEnemyDead(AActor* DeadActor);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAdditionalEnemiesDead(AActor* DeadActor);

protected:
	virtual void ExecutePlayMontageAndWait() override;

	UPROPERTY(EditDefaultsOnly, Category="Electrocute")
	int32 MaxNumElectrocute = 5;

	UPROPERTY(EditDefaultsOnly, Category="Electrocute")
	float ChainEffectActivateRange = 500.f;

	UPROPERTY(EditDefaultsOnly, Category="Electrocute")
	float DamageDeltaTime = 0.3f;
};

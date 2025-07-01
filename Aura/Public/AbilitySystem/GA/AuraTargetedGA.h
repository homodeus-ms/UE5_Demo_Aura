// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GA/AuraGABase.h"
#include "Interface/CombatInterface.h"
#include "AuraTargetedGA.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetReadyDelegate);

/**
 * 
 */
UCLASS()
class AURA_API UAuraTargetedGA : public UAuraGABase
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable)
	virtual void SetTargetActor();

	UFUNCTION(BlueprintCallable)
	void UpdateFacing();

	UFUNCTION(BlueprintCallable)
	void PlayMontageAndWaitForRandomTag();

	UFUNCTION(BlueprintCallable)
	void PlayMontageAndWaitForSpecificTag(FGameplayTag MontageTag);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnReceiveEventTag(FGameplayEventData Payload);

	UFUNCTION(BlueprintCallable)
	void SetCharacterMovementValid(AActor* Actor, bool bValid);

	UPROPERTY(BlueprintAssignable)
	FOnTargetReadyDelegate OnTargetReadyDelegate;

protected:
	UFUNCTION(BlueprintCallable)
	bool IsTargetInSkillRange();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TScriptInterface<class ICombatInterface> CombatInterfaceActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	FVector TargetLocation;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	FTaggedMontage TaggedMontage;

	UPROPERTY(EditDefaultsOnly)
	float SkillRange = 0.f;

	virtual void ExecutePlayMontageAndWait();

	UFUNCTION()
	void OnEndTask();

	UFUNCTION()
	void OnInterruptedTask();

	UFUNCTION()
	void OnCancelledTask();

	FTimerHandle WaitTimeOver;

	UPROPERTY()
	TObjectPtr<class UAbilityTask_PlayMontageAndWait> PlayAndWaitTask;

private:
	UFUNCTION()
	void OnCursorTargetDataCallback(const FGameplayAbilityTargetDataHandle DataHandle);

	UFUNCTION()
	void OnWaitTimeEnded();
	
};

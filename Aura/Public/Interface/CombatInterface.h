// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "CombatInterface.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegisteredDelegate, class UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeadDelegate, AActor*, DeadActor);

class UAnimMontage;
class USoundBase;

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttackMontageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttackSocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> ImpactSound;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

public:
	virtual uint8 GetCharacterLevel() abstract;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	class USkeletalMeshComponent* GetWeaponComponent() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag) const;

	// If Have More than One, then Return Randomly 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetAttackMontageFromArray() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetAttackMontageByMontageTag(FGameplayTag MontageTag) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	class UNiagaraSystem* GetBloodEffect() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void FacingTarget(FVector Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsFriend(const AActor* OtherActor) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetShockLoop(bool bLooping);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsShockLoop() const;

	virtual bool IsPlayer() const abstract;

	virtual void Die() abstract; 

	virtual bool IsTargetInAttackRange(AActor* InTargetActor) const abstract;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnComboAttackCompleted();

	virtual FOnASCRegisteredDelegate& GetOnASCRegisteredDelegate() = 0;
	virtual FOnDeadDelegate& GetOnDeadDelegate() = 0;

};

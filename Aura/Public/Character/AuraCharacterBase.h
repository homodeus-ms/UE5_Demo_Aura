// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/CombatInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraCharacterBase.generated.h"

class UGameplayEffect;
class UGameplayAbility;
class UAnimMontage;
class UNiagaraSystem;
class UAuraNiagaraComponent;

UCLASS()
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	
	AAuraCharacterBase(const FObjectInitializer& ObjectInitializer);

	// Interface Functions
	// IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// CombatInterface
	virtual class USkeletalMeshComponent* GetWeaponComponent_Implementation() const override;
	virtual uint8 GetCharacterLevel() override { return 0.f; }
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) const override;
	virtual FTaggedMontage GetAttackMontageFromArray_Implementation() const override;
	virtual FTaggedMontage GetAttackMontageByMontageTag_Implementation(FGameplayTag MontageTag) const override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() const override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() const override;
	
	virtual void Die() override;
	virtual bool IsDead_Implementation() const override; 
	virtual AActor* GetAvatar_Implementation() override;
	virtual bool IsFriend_Implementation(const AActor* OtherActor) const override;
	virtual bool IsPlayer() const override { return ActorHasTag(FName(TEXT("Player"))); }
	virtual FOnASCRegisteredDelegate& GetOnASCRegisteredDelegate() override { return OnASCRegistered; }
	virtual FOnDeadDelegate& GetOnDeadDelegate() override { return OnDeadDelegate; }
	virtual void SetShockLoop_Implementation(bool bLooping) override;
	virtual bool IsShockLoop_Implementation() const override;
	virtual bool IsTargetInAttackRange(AActor* InTargetActor) const override { return true; }
	
	virtual void OnComboAttackCompleted_Implementation() override {};

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	class UAttributeSet* GetAttributeSet() const;
	ECharacterType GetCharacterType() const { return CharacterType; }
	void SetCharacterType(ECharacterType InType) { CharacterType = InType; }
	virtual void SetLevel(int32 InLevel) {}

protected:

	virtual void PossessedBy(AController* NewController) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void InitAbilitySystem() {}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FOnDeadDelegate OnDeadDelegate;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHandleDeath();

	UFUNCTION(BlueprintCallable)
	void OnHitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(EditAnywhere, Category="NeedToSetUp|Combat")
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	UPROPERTY(VisibleAnywhere, Category="GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, Category="Attribute")
	TObjectPtr<class UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NeedToSetUp|Character Class Defaults")
	ECharacterType CharacterType = ECharacterType::Warrior;

	UPROPERTY(EditDefaultsOnly, Category= "NeedToSetUp|Combat")
	TArray<FTaggedMontage> AttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "NeedToSetUp|Combat")
	TMap<FGameplayTag, FName> CombatSockets;

	UPROPERTY(EditDefaultsOnly, Category="NeedToSetUp|Montage")
	TObjectPtr<UAnimMontage> HitReaction;

	UPROPERTY(BlueprintReadOnly, Category = "NeedToSetUp|Combat")
	bool bHitReacting = false;

	UPROPERTY(EditDefaultsOnly, Category = "NeedToSetUp|Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditDefaultsOnly, Category = "NeedToSetUp|BloodEffect")
	TObjectPtr<UNiagaraSystem> BloodEffect;

	bool bDead = false;


	// Dissolve Effect
	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveCharacter(UMaterialInstanceDynamic* InDynamicMaterial);

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveWeapon(UMaterialInstanceDynamic* InDynamicMaterial);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NeedToSetUp|Dissolve")
	TObjectPtr<class UMaterialInstance> CharacterDissolveMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NeedToSetUp|Dissolve")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterial;

	// Debuff Effect
	FOnASCRegisteredDelegate OnASCRegistered;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAuraNiagaraComponent> BurnDebuffEffectComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAuraNiagaraComponent> StunDebuffEffectComponent;


	// GA Lightning

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bIsShockLoop = false;

	UPROPERTY(ReplicatedUsing = OnRep_Stunned, BlueprintReadOnly)
	bool bIsStunned = false;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION()
	virtual void OnRep_Stunned() {}

private:
	void RemoveDebuffsOnDead();
	
};

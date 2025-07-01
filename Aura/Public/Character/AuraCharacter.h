// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interface/PlayerInterface.h"
#include "GameplayTagContainer.h"
#include "AuraCharacter.generated.h"

class UAuraNiagaraComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
	
public:

	AAuraCharacter(const FObjectInitializer& ObjectInitializer);

	// PlayerInterface 
	UFUNCTION(NetMulticast, Reliable)
	virtual void ShowDamageText(float DamageNum, bool bBlocked, bool bCritical, ACharacter* Target) override;
	
	virtual class AAuraPlayerController* GetAuraPlayerController() const override;
	virtual class AAuraPlayerState* GetAuraPlayerState() override;
	virtual int32 GetNewLevelForXP(int32 XP) override;
	virtual int32 GetCurrentXP() override;
	virtual int32 GetCurrentGold() override;
	virtual int32 GetAttributePoint() override;
	virtual int32 GetSpellPoint() override;
	virtual void SpendItem(const FGameplayTag& ItemTag) override;
	virtual void RecieveBoxItem(const FGameplayTag& ItemTag, int32 ItemAmount) override;
	virtual bool IsSiphonEffectActivate() override;

	virtual void AddToXP(int32 XP) override;
	virtual void AddGold(int32 Gold) override;
	virtual void GetItem(const FGameplayTag& ItemSpendTag, int32 Amount = 1) override;
	virtual void AddToSpellPoint(int32 AddPoint) override;
	virtual void UpdateLevel(int32 NewLevel) override;
	virtual void UpdateAttributePoint(int32 Value) override;
	virtual bool IsLevelUpCondition(int32 InXP) override;
	virtual void HandleLevelUp() override;

	virtual bool IsHaloOfProtectionActive() override;
	virtual bool IsHealthSiphonActive() override;
	virtual bool IsManaSiphonActive() override;
	virtual void HandleSiphonEffect(float InDamage) override;

	virtual void ShowMagicCircle_Implementation(class UMaterialInterface* DecalMaterial = nullptr) override;
	virtual void HideMagicCircle_Implementation() override;

	virtual void SaveCurrentGame(const FString& LevelName, const FName& PlayerStartName) override;
	virtual void LoadPlayerData(bool IsFirstLoadIn) override;
	virtual bool IsInvincible() override { return bIsInvincible; }

	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual void OnRep_PlayerState();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerStateReplicated();

	// CharacterBase
	
	virtual uint8 GetCharacterLevel() override;
	virtual void Die() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

protected:
	virtual void InitAbilitySystem() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SpringArm)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NameTag")
	TObjectPtr<class UWidgetComponent> NameTagWidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category = "NameTag")
	TSubclassOf<class UUserWidget> NameTagWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetComponent")
	TSubclassOf<class UDamageTextWidgetComponent> FlowtingTextWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Level|LevelUpEffect")
	TObjectPtr<class UNiagaraComponent> LevelUpEffect;

	UPROPERTY(EditDefaultsOnly, Category="NeedToSetup|PlayerPassiveAbilities")
	TArray<TSubclassOf<class UGameplayAbility>> PassiveAbilities;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	virtual void OnRep_Stunned() override;

	// Passive Skill Effect
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAuraNiagaraComponent> HaloOfProtectionEffectComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAuraNiagaraComponent> HealthSiphonEffectComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAuraNiagaraComponent> ManaSiphonEffectComponent;

	UPROPERTY(EditDefaultsOnly, Category="NeedToSetup|SiphonEffect")
	TSubclassOf<class UGameplayEffect> HealthSiphonEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "NeedToSetup|SiphonEffect")
	TSubclassOf<class UGameplayEffect> ManaSiphonEffectClass;

private:

	UFUNCTION(NetMulticast, Reliable)
	void MulticastShowLevelUpEffect() const;

	TObjectPtr<class AAuraPlayerState> AuraPS = nullptr;

	void AddBlockInputLooseTagsWhenStunned();
	void RemoveBlockInputLooseTagsRecoverStunned();

	FGameplayTagContainer BlockInputTagsContainer;

	// OnSpawn
	FTimerHandle OnSpawnProtectionTimer;

	UPROPERTY(EditDefaultsOnly)
	int32 OnSpawnProtectionTime = 3.f;

	UPROPERTY(Replicated)
	bool bIsInvincible = false;

	// OnDead
	UFUNCTION(Client, Reliable)
	void ShowDeadMessage();

	UFUNCTION(Client, Reliable)
	void ReSpawn();

	UPROPERTY(EditDefaultsOnly, Category = "UI|PlayerDead")
	TSubclassOf<class UUserWidget> DeadMessageWidgetClass;

	FTimerHandle DeadTimer;

	UPROPERTY(EditDefaultsOnly)
	int32 DeadDelayTime = 5.f;

	UPROPERTY(EditDefaultsOnly)
	float AttackRange;
};

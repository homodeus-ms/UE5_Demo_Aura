// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UGameplayAbility;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEffectAssetTagsApplied, const FGameplayTagContainer& /* AssetTags */);
DECLARE_MULTICAST_DELEGATE(FOnStartupAbilitySet);
DECLARE_DELEGATE_OneParam(FGetEachAbilitySpecFuncType, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /* AbilityTag */, const FGameplayTag& /* StatusTag */, int32 /* AbilityLevel */);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipChanged, const FGameplayTag& /* AbilityTag */, const FGameplayTag& /* StatusTag */, const FGameplayTag& /* InputTag */, const FGameplayTag& /* PrevSlotInputTag */);
DECLARE_MULTICAST_DELEGATE_OneParam(FPassiveSkillDeactivate, const FGameplayTag& /* AbilityTag */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FActivatePassiveSkillEffect, const FGameplayTag& /* AbilityTag */, bool /* bActivate */);

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UAuraAbilitySystemComponent();

	FOnEffectAssetTagsApplied OnEffectAssetTagsApplied;
	FOnStartupAbilitySet OnStartupAbilitySetDelegate;
	FAbilityStatusChanged OnAbilityStatusChanged;
	FAbilityEquipChanged OnAbilityEquipChanged;
	FPassiveSkillDeactivate OnPassiveSkillDeactivate;
	FActivatePassiveSkillEffect OnActivatePassiveSkillEffect;

	
	void InitSlotToEquippedSpecMap();
	FGameplayTag GetStatusTagFromAbilitySpec(const FGameplayAbilitySpec& Spec);
	FGameplayTag GetAbilityTagFromAbilitySpec(const FGameplayAbilitySpec& Spec);
	FGameplayTag GetSlotTagFromAbilitySpec(const FGameplayAbilitySpec& Spec);
	FGameplayTag GetStatusTagFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayAbilitySpec* GetSpecFromActivatableAbilitiesByTagOrNull(const FGameplayTag& AbilityTag);
	bool IsPassiveAbility(const FGameplayTag& AbilityTag);
	bool IsEmptySlot(const FGameplayTag& SlotTag);
	FGameplayAbilitySpecHandle GetSpecHandleInSlot(const FGameplayTag& SlotTag);
	void ClearSlot(const FGameplayTag& SlotTag, FGameplayAbilitySpec* SlotSpec);

	bool GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag, const class UAbilityInfo* AbilityInfo, FString& OUTDesc, FString& OUTNextLevelDesc);

	FORCEINLINE bool IsStartupAbilitySet() const { return bStartupAbilitySet; }
	void GetEachAbilitySpec(const FGetEachAbilitySpecFuncType& Callback);

	void AbilityActorInfoSet();

	void AddAbilities(const TArray<TSubclassOf<UGameplayAbility>> Abilities);
	void AddPassiveStartupAbilities(const TArray<TSubclassOf<UGameplayAbility>> Abilities);
	void AddAbilitiesWithTag(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddAbilitiesWithTag(FGameplayAbilitySpec& AbilitySpec);
	void AddAbilityOnLevel(int32 Level);
	void AddPlayerAbilitiesFromLoadedData(class UAuraSaveData* SaveData);

	void ActivateAbilityOnPressed(const FGameplayTag& Tag);
	void ActivateAbilityOnHeld(const FGameplayTag& Tag);
	void DeActivateAbilityOnReleased(const FGameplayTag& Tag);

	virtual void OnRep_ActivateAbilities() override;
	void IncreaseAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);

	UFUNCTION(Server, Reliable)
	void ServerEquipSkill(const FGameplayTag& AbilityTag, const FGameplayTag& TargetSlotTag, const FGameplayTag& StatusTag);

	UFUNCTION(Client, Reliable)
	void ClientEquipSkill(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& TargetSlotTag, const FGameplayTag& PrevSlotTag);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastActivatePassiveEffect(const FGameplayTag& AbilityTag, bool bActivate);

	

protected:

	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	UFUNCTION(Server, Reliable)
	void ServerIncreaseAttribute(const FGameplayTag& AttributeTag);
	
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);

	bool bStartupAbilitySet = false;

private:
	void ChangeSpecAttachedTag(FGameplayAbilitySpec* Spec, const FGameplayTag& TagToRemove, const FGameplayTag& TagToAdd);
	void UpdateSlotToEquippedSpecMap(const FGameplayTag& InputTag, const FGameplayAbilitySpecHandle& SpecHandle);
	
	TMap<FGameplayTag, FGameplayAbilitySpecHandle> SlotToEquippedSpecOrNull;
};

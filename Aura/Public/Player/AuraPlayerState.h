// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DedicatedServer/Public/Player/DS_PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "AuraTypes.h"
#include "Data/StageClearCondition.h"
#include "AuraPlayerState.generated.h"

class LevelUpInfo;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatChangedDelegate, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChangedDelegate, int32 /* Level */, bool /* bLevelUp */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGoldChangedDelegate, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInventoryChangedDelegate, const FGameplayTag& /* InventoryTag */, int32 /* Amount */);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnGetItemFromTreasureBoxDelegate, const FGameplayTag& /* ItemTag */, int32 /* AMount */, bool /* bIsLoadedData */);

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public ADS_PlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AAuraPlayerState();

	// Interface Functions
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	class UAttributeSet* GetAttributeSet() const;
	FORCEINLINE int32 GetPlayerLevel() const { return PlayerLevel; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoint() const { return AttributePoint; }
	FORCEINLINE int32 GetSpellPoint() const { return SpellPoint; }
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE const TMap<FGameplayTag, int32>& GetInventoryItems() const { return InventoryItems; }
	FORCEINLINE const TArray<FGameplayTag>& GetCollections() const { return Collections; }

	void AddXP(int32 EarnXP);
	void UpdateLevel(int32 NewLevel);
	void AddAttributePoint(int32 AddPoint);
	void AddSpellPoint(int32 AddPoint);
	void AddGold(int32 InGold);
	void SpendItem(const FGameplayTag& ItemSpendTag);
	void BuyItem(const TArray<TPair<FGameplayTag, int32>>& Items, int32 SpentGold);

	void SetPlayerLevelByLoadedData(int32 Level);
	void SetXPByLoadedData(int32 InXP);
	void SetGoldByLoadedData(int32 InGold);
	void SetInventoryItems(const TMap<FGameplayTag, int32>& Items);
	void SetCollectionsByLoadedData(const TArray<FGameplayTag>& InCollections);

	UFUNCTION(Server, Reliable)
	void Server_PurchaseItem(const TArray<FInventoryItemEntry>& Items, int32 TotalPrice);

	bool AddInventoryItemByItemSpendTag(const FGameplayTag& ItemSpendTag, int32 InAmount);
	void AddInventoryItemFromTreasureBox(const FGameplayTag& ItemTag, int32 InAmount);
	void AddCollectingItem(const FGameplayTag& ItemTag);
	
	
	class ULevelUpInfo* GetLevelUpInfo() const { return LevelUpInfo; }
	bool IsLevelUpCondition(int32 NewXP);
	bool HasEnoughGold(int32 GoldToSpend) { return Gold >= GoldToSpend; }
	int32 GetNewLevelForXP(int32 InXP);

	FOnStatChangedDelegate OnXPChangedDelegate;
	FOnLevelChangedDelegate OnLevelChangedDelegate;
	FOnStatChangedDelegate OnAttributePointChangedDelegate;
	FOnStatChangedDelegate OnSpellPointChangedDelegate;
	FOnGoldChangedDelegate OnGoldChangedDelegate;
	FOnInventoryChangedDelegate OnInventoryChangedDelegate;
	FOnGetItemFromTreasureBoxDelegate OnGetItemFromTreasureBox;

	void LoadNewPlayerDatas();

	void OnRetrieveData(UAuraSaveData* InSaveData);

protected:

	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AttributeSet")
	TObjectPtr<class UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category="LevelUp")
	TObjectPtr<class ULevelUpInfo> LevelUpInfo;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_PlayerLevel)
	int32 PlayerLevel = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	int32 XP = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoint)
	int32 AttributePoint = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoint)
	int32 SpellPoint = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Gold)
	int32 Gold = 100;

	UPROPERTY(EditDefaultsOnly, Category="InventoryItem")
	TSubclassOf<class ABuyableItem> ItemForKey5;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryItem")
	TSubclassOf<class ABuyableItem> ItemForKey6;

	UPROPERTY(EditDefaultsOnly, Category="InventoryItemInfo")
	TObjectPtr<class UInventoryItemInfo> InventoryItemInfo;

	UPROPERTY(ReplicatedUsing = OnRep_InventoryItemArray)
	TArray<FInventoryItemEntry> InventoryItemArray;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryItem")
	TMap<FGameplayTag, int32> InventoryItems;

	UPROPERTY(EditDefaultsOnly, Category="CollectingItemInfo")
	TObjectPtr<class UCollectingItemInfo> CollectingItemInfo;

	UPROPERTY(EditDefaultsOnly, Category = "CollectingItem")
	TArray<FGameplayTag> Collections;

	UPROPERTY(ReplicatedUsing = OnRep_AddedCollectingItem)
	FGameplayTag AddedCollectingItem{};

	UFUNCTION()
	void OnRep_PlayerLevel();

	UFUNCTION()
	void OnRep_XP();

	UFUNCTION()
	void OnRep_AttributePoint();

	UFUNCTION()
	void OnRep_SpellPoint();

	UFUNCTION()
	void OnRep_Gold();

	UFUNCTION()
	void OnRep_InventoryItemArray();

	UFUNCTION()
	void OnRep_AddedCollectingItem();

	UFUNCTION(Client, Reliable)
	void Client_OnGetItemFromTreasureBox(const FGameplayTag& ItemTag, int32 InAmount);

	UFUNCTION(Client, Reliable)
	void Client_SetCollectionsByLoadedData(const TArray<FGameplayTag>& InCollections);

private:
	void UpdateEarnPointsAndBroadcast(int32 OldLevel, int32 NewLevel);
	int32 GetInventoryItemCount(const FGameplayTag& ItemTag) const;
	
	bool HasCollectingItemAlready(const FGameplayTag& ItemTag);
	int32 MaxLevel;
	
	int32 PrevLevel = -1;

	TArray<FInventoryItemEntry> PrevInventory;
};

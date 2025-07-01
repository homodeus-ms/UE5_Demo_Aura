// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	virtual void ShowDamageText(float DamageNum, bool bBlocked, bool bCritical, ACharacter* Target) abstract;
	
	virtual class AAuraPlayerController* GetAuraPlayerController() const abstract;
	
	virtual int32 GetCurrentXP() abstract;
	virtual int32 GetCurrentGold() abstract;
	virtual int32 GetNewLevelForXP(int32 XP) abstract;
	virtual int32 GetAttributePoint() abstract;
	virtual int32 GetSpellPoint() abstract;
	virtual void SpendItem(const FGameplayTag& ItemTag) abstract;
	virtual void RecieveBoxItem(const FGameplayTag& ItemTag, int32 ItemAmount) abstract;
	virtual bool IsSiphonEffectActivate() abstract;

	virtual void AddToXP(int32 XP) abstract;
	virtual void AddGold(int32 Gold) abstract;
	virtual void GetItem(const FGameplayTag& ItemTag, int32 Amount = 1) abstract;
	virtual void AddToSpellPoint(int32 AddPoint) abstract;
	virtual void UpdateLevel(int32 NewLevel) abstract;
	virtual void HandleLevelUp() abstract;
	virtual void UpdateAttributePoint(int32 Value) abstract;
	virtual bool IsInvincible() abstract;

	virtual class AAuraPlayerState* GetAuraPlayerState() abstract;
	virtual bool IsLevelUpCondition(int32 InXP) abstract;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowMagicCircle(class UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideMagicCircle();

	// Check Passive Skill
	virtual bool IsHaloOfProtectionActive() abstract;
	virtual bool IsHealthSiphonActive() abstract;
	virtual bool IsManaSiphonActive() abstract;
	virtual void HandleSiphonEffect(float InDamage) abstract;

	// Save and Load
	virtual void SaveCurrentGame(const FString& LevelName, const FName& PlayerStartName) abstract;
	virtual void LoadPlayerData(bool IsFirstLoadIn = false) abstract;
};

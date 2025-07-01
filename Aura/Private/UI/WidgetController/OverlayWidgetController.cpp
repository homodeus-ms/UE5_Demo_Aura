// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AuraLog.h"
#include "AuraGameplayTags.h"



void UOverlayWidgetController::BroadcastInitialValues()
{
	
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());

	OnGoldChanged.Broadcast(AuraPlayerState->GetGold());
	const TMap<FGameplayTag, int32>& Items = AuraPlayerState->GetInventoryItems();
	for (const auto& Pair : Items)
	{ 
		OnInventoryChanged.Broadcast(Pair.Key, Pair.Value);
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{

	BIND_ATTRIBUTE_CHANGE_DELEGATE(Health);
	BIND_ATTRIBUTE_CHANGE_DELEGATE(Mana);
	BIND_ATTRIBUTE_CHANGE_DELEGATE(MaxHealth);
	BIND_ATTRIBUTE_CHANGE_DELEGATE(MaxMana);

	if (AuraASC)
	{
		AuraASC->OnEffectAssetTagsApplied.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					// 'Message.a' MatchesTag 'Message' return true
					const FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Item"));
					if (Tag.MatchesTag(MessageTag))
					{
						FMessageWidgetRow* Row =
							GetDataTableRowByTag<FMessageWidgetRow>(MessageWidgetRowDataTable, Tag);
						Row->Amount = 1;

						if (Row)
							OnMessageWidgetRow.Broadcast(*Row, false);
					}
				}
			});

		// Because Not Sure Timing Between 'Startup Ability of ASC' and 'Bind Delegate'  
		if (AuraASC->IsStartupAbilitySet())
		{
			BroadcastAbilityInfoChanged();
		}
		else
		{
			AuraASC->OnStartupAbilitySetDelegate.AddUObject(this, &UAuraWidgetController::BroadcastAbilityInfoChanged);
		}
	}

	AuraPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXpChangedCallback);
	AuraPlayerState->OnLevelChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnLevelChangedCallback);
	AuraPlayerState->OnGoldChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnGoldChangedCallback);
	AuraPlayerState->OnInventoryChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnInventoryChangedCallback);
	AuraPlayerState->OnGetItemFromTreasureBox.AddUObject(this, &UOverlayWidgetController::OnGetItemFromTreasureBoxCallback);

	AuraASC->OnAbilityEquipChanged.AddUObject(this, &ThisClass::OnAbilityEquipChanged);
}


void UOverlayWidgetController::OnXpChangedCallback(int32 NewXP)
{
	float Percent = CalculateXPPercent(NewXP);
	XPBarChangedDelegate.Broadcast(Percent);
}

void UOverlayWidgetController::OnLevelChangedCallback(int32 NewValue, bool bLevelUp)
{
	LevelChangedDelegate.Broadcast(NewValue, bLevelUp);
}

void UOverlayWidgetController::OnGetItemFromTreasureBoxCallback(const FGameplayTag& ItemTag, int32 InAmount, bool bLoadData)
{
	UE_LOG(Aura, Log, TEXT("OnGetItemFromTreasureBox, ItemTag: %s, InAmount: %d"), *ItemTag.ToString(), InAmount);
	FMessageWidgetRow* Row =
		GetDataTableRowByTag<FMessageWidgetRow>(MessageWidgetRowDataTable, ItemTag);
	Row->Amount = InAmount;

	OnMessageWidgetRow.Broadcast(*Row, bLoadData);
}


float UOverlayWidgetController::CalculateXPPercent(int32 NewXP)
{
	ULevelUpInfo* LevelUpInfo = AuraPlayerState->GetLevelUpInfo();

	int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	int32 CurrentMaxXP = LevelUpInfo->GetCurrentLevelMaxXP(Level);
	int32 PreMaxXP = Level > 1 ? LevelUpInfo->GetCurrentLevelMaxXP(Level - 1) : 0;

	float Denominator = static_cast<float>(CurrentMaxXP - PreMaxXP);
	check(Denominator != 0.f);
	int32 Numerator = NewXP - PreMaxXP;
	check(Numerator >= 0);
	
	return Numerator / Denominator;
}





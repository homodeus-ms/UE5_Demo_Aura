// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AuraLog.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"
#include "UI/Widgets/AuraUserWidget.h"

USpellMenuWidgetController::USpellMenuWidgetController()
{
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	SelectedSpellBoxTag.AbilityTag = FGameplayTag::EmptyTag;
	SelectedSpellBoxTag.StatusTag = FGameplayTag::EmptyTag;
}

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfoChanged();

	OnSpellPointChangedDelegate.Broadcast(0);
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	if (AuraASC->IsStartupAbilitySet())
	{
		BroadcastAbilityInfoChanged();
	}
	else
	{
		AuraASC->OnStartupAbilitySetDelegate.AddUObject(this, &UAuraWidgetController::BroadcastAbilityInfoChanged);
	}

	AuraPlayerState->OnSpellPointChangedDelegate.AddLambda([this](int32 NewSpellPoint)
		{
			OnSpellPointChangedDelegate.Broadcast(NewSpellPoint);

			CurrentSpellPoint = NewSpellPoint;

			bool SpendPointBoxEnabled = false;
			bool EquipBoxEnabled = false;
			ShouldEnableButton(SelectedSpellBoxTag.AbilityTag, SelectedSpellBoxTag.StatusTag, SpendPointBoxEnabled, EquipBoxEnabled);

			FString Desc1;
			FString Desc2;
			AuraASC->GetDescriptionByAbilityTag(SelectedSpellBoxTag.AbilityTag, AbilityInfo, Desc1, Desc2);
			OnBoxSelected.Broadcast(SpendPointBoxEnabled, EquipBoxEnabled, Desc1, Desc2);
		});
	
	AuraASC->OnAbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
		{
			if (SelectedSpellBoxTag.AbilityTag.MatchesTagExact(AbilityTag))
			{
				SelectedSpellBoxTag.StatusTag = StatusTag;
				bool SpendPointBoxEnabled = false;
				bool EquipBoxEnabled = false;
				ShouldEnableButton(AbilityTag, StatusTag, SpendPointBoxEnabled, EquipBoxEnabled);

				FString Desc1;
				FString Desc2;
				AuraASC->GetDescriptionByAbilityTag(AbilityTag, AbilityInfo, Desc1, Desc2);
				OnBoxSelected.Broadcast(SpendPointBoxEnabled, EquipBoxEnabled, Desc1, Desc2);
			}

			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityByAbilityTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}

			// Todo : Broadcast About AbilityLevel
		});

	AuraASC->OnAbilityEquipChanged.AddUObject(this, &ThisClass::OnAbilityEquipChanged);
}

void USpellMenuWidgetController::SpendPoint()
{
	AuraASC->ServerSpendSpellPoint(SelectedSpellBoxTag.AbilityTag);
}

void USpellMenuWidgetController::EquipSpellSkill(const FGameplayTag& NewSelectBoxTag)
{
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	check(SelectedSpellBoxTag.AbilityTag.IsValid() &&
		!SelectedSpellBoxTag.AbilityTag.MatchesTagExact(AuraTags.Abilities_None));

	AuraASC->ServerEquipSkill(SelectedSpellBoxTag.AbilityTag, NewSelectBoxTag, SelectedSpellBoxTag.StatusTag);
}



void USpellMenuWidgetController::SpellBoxSelected(const FGameplayTag& AbilityTag)
{
	const FAuraGameplayTags AuraTags = FAuraGameplayTags::Get();

	if (AbilityTag.MatchesTagExact(AuraTags.Abilities_None))
	{
		return;
	}

	if (SelectedSpellBoxTag.AbilityTag.MatchesTagExact(AbilityTag))
	{
		ResetCahcedBox();
		return;
	}

	FGameplayAbilitySpec* Spec = AuraASC->GetSpecFromActivatableAbilitiesByTagOrNull(AbilityTag);

	bool bValidTag = AbilityTag.IsValid();
	bool bTagAbilityNone = AbilityTag.MatchesTag(AuraTags.Abilities_None);
	bool bSpecInActivatable = Spec != nullptr;

	FGameplayTag StatusTag;

	if (!bValidTag || bTagAbilityNone || !bSpecInActivatable)
	{
		StatusTag = AuraTags.Abilities_Status_Locked;
	}
	else
	{
		StatusTag = AuraASC->GetStatusTagFromAbilitySpec(*Spec);
	}

	SelectedSpellBoxTag.AbilityTag = AbilityTag;
	SelectedSpellBoxTag.StatusTag = StatusTag;

	bool SpendPointBoxEnabled = false;
	bool EquipBoxEnabled = false;
	ShouldEnableButton(AbilityTag, StatusTag, SpendPointBoxEnabled, EquipBoxEnabled);
	
	FString Desc1;
	FString Desc2;
	AuraASC->GetDescriptionByAbilityTag(AbilityTag, AbilityInfo, Desc1, Desc2);

	OnBoxSelected.Broadcast(SpendPointBoxEnabled, EquipBoxEnabled, Desc1, Desc2);
}

void USpellMenuWidgetController::ResetCahcedBox()
{
	SelectedSpellBoxTag.AbilityTag = FGameplayTag::EmptyTag;
	SelectedSpellBoxTag.StatusTag = FGameplayTag::EmptyTag;
	
	OnBoxSelected.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::ShouldEnableButton(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, bool& SpendPointButton, bool& EquipButton)
{
	const FAuraGameplayTags AuraTags = FAuraGameplayTags::Get();

	// InCase Status Locked
	if (StatusTag == AuraTags.Abilities_Status_Locked)
	{
		SpendPointButton = false;
		EquipButton = false;
		return;
	}
	
	SpendPointButton = AuraPlayerState->GetSpellPoint() > 0 && SelectedSpellBoxTag.AbilityTag.IsValid();

	bool ResultForEquipButton = false;
	if (StatusTag == AuraTags.Abilities_Status_Eligible)
	{
		ResultForEquipButton = false;
	}
	else if (StatusTag == AuraTags.Abilities_Status_Equipped)
	{
		ResultForEquipButton = true;
	}
	else if (StatusTag == AuraTags.Abilities_Status_UnLocked)
	{
		ResultForEquipButton = true;
	}
	
	EquipButton = ResultForEquipButton;
}

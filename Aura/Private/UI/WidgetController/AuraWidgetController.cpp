// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AuraGameplayTags.h"


void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	ASC = Params.ASC;
	AttributeSet = Params.AttributeSet;

	// Cache
	GetAuraPlayerState();
	GetAuraASC();
	GetAuraAttributeSet();
}

AAuraPlayerState* UAuraWidgetController::GetAuraPlayerState()
{
	if (!AuraPlayerState)
	{
		AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
		return AuraPlayerState;
	}
	return AuraPlayerState;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraASC()
{
	if (!AuraASC)
	{
		AuraASC = Cast<UAuraAbilitySystemComponent>(ASC);
		return AuraASC;
	}
	return AuraASC;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAttributeSet()
{
	if (!AuraAttributeSet)
	{
		AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
		return AuraAttributeSet;
	}
	return AuraAttributeSet;
}

void UAuraWidgetController::BroadcastAbilityInfoChanged()
{
	if (!GetAuraASC()->IsStartupAbilitySet())
		return;

	FGetEachAbilitySpecFuncType Callback;
	Callback.BindLambda([this](const FGameplayAbilitySpec& Spec)
		{
			FGameplayTag AbilityTag = AuraASC->GetAbilityTagFromAbilitySpec(Spec);
			FGameplayTag AbilityInputTag = AuraASC->GetSlotTagFromAbilitySpec(Spec);
			FGameplayTag StatusTag = AuraASC->GetStatusTagFromAbilitySpec(Spec);

			FAuraAbilityInfo Info = AbilityInfo->FindAbilityByAbilityTag(AbilityTag);
			Info.InputTag = AbilityInputTag;
			Info.StatusTag = StatusTag;

			AbilityInfoDelegate.Broadcast(Info);
		});

	AuraASC->GetEachAbilitySpec(Callback);
}

void UAuraWidgetController::OnAbilityEquipChanged(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& InputTag, const FGameplayTag& PrevBoxInputTagOrEmpty)
{
	if (!AbilityInfo)
		return;

	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

	FAuraAbilityInfo PrevSlotInfo;
	PrevSlotInfo.AbilityTag = AuraTags.Abilities_None;
	PrevSlotInfo.StatusTag = AuraTags.Abilities_Status_UnLocked;
	PrevSlotInfo.InputTag = PrevBoxInputTagOrEmpty;
	AbilityInfoDelegate.Broadcast(PrevSlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityByAbilityTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = InputTag;
	AbilityInfoDelegate.Broadcast(Info);
}

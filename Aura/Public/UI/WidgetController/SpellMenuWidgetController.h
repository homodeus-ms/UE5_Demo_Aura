// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "SpellMenuWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FBoxSelectedDelegate, bool, SpendPointButtonEnabled, bool, EquipButtonEnabled, FString, Desc1, FString, Desc2);

/**
 * 
 */

struct FSelectedBoxTag
{
	FGameplayTag AbilityTag = FGameplayTag::EmptyTag;
	FGameplayTag StatusTag = FGameplayTag::EmptyTag;
};

UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	USpellMenuWidgetController();

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetSelectedSpellBoxAbilityTag() const { return SelectedSpellBoxTag.AbilityTag; }

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FStatChangeDelegate OnSpellPointChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|SpellMenu")
	FBoxSelectedDelegate OnBoxSelected;

	UFUNCTION(BlueprintCallable)
	void SpendPoint();

	UFUNCTION(BlueprintCallable)
	void EquipSpellSkill(const FGameplayTag& NewSelectBoxTag);
	

protected:

	UFUNCTION(BlueprintCallable)
	void SpellBoxSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void ResetCahcedBox();


private:
	void ShouldEnableButton(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, bool& SpendPointButton, bool& EquipButton);
	FSelectedBoxTag SelectedSpellBoxTag;
	
	int32 CurrentSpellPoint = 0;

};

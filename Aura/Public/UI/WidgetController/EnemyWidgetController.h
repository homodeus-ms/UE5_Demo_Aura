// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "EnemyWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttibuteChangedDelegate, const float, NewValue);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UEnemyWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttibuteChangedDelegate OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttibuteChangedDelegate OnMaxHealthChanged;
};

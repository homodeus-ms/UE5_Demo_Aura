// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/EnemyWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraLog.h"

void UEnemyWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
}

void UEnemyWidgetController::BindCallbacksToDependencies()
{
	ASC->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	ASC->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FAuraAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoDelegate, const struct FAuraAttributeInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	void IncreaseAttribute(const FGameplayTag& AttributeTag);

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoDelegate OnAttributeInfoChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FStatChangeDelegate OnAttributePointChangedDelegate;



protected:
	UFUNCTION()
	void AttributePointChangedCallback(int32 NewValue);
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfoChangedDelegate(const FGameplayTag& Tag, const struct FGameplayAttribute& Attribute);
};

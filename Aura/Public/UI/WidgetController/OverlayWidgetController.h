// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/InventoryDataController.h"
#include "GameplayEffectTypes.h"
#include "OverlayWidgetController.generated.h"

class UAbilityInfo;

USTRUCT(BlueprintType)
struct FMessageWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UAuraUserWidget> MessageWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Amount = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedDelegate, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMessageWidgetRowDelegate, FMessageWidgetRow, Row, bool, bLoadData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FXPBarChangeDelegate, float, Percent);




#define BIND_ATTRIBUTE_CHANGE_DELEGATE(AttributeName) \
ASC->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->Get##AttributeName##Attribute()).AddLambda( \
	[this](const FOnAttributeChangeData& Data) \
	{ \
		On##AttributeName##Changed.Broadcast(Data.NewValue); \
	}); 


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UInventoryDataController
{
	GENERATED_BODY()
	
public:

	// Inventory Data Provider Implements

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedDelegate OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedDelegate OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedDelegate OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedDelegate OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnMessageWidgetRowDelegate OnMessageWidgetRow;

	UPROPERTY(BlueprintAssignable, Category = "GAS|LevelUp")
	FXPBarChangeDelegate XPBarChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|LevelUp")
	FLevelChangeDelegate LevelChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|LevelUp")
	FStatChangeDelegate AttributePointChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|LevelUp")
	FStatChangeDelegate SpellPointChangedDelegate;

	
protected:
	UFUNCTION()
	void OnXpChangedCallback(int32 NewXP);

	UFUNCTION()
	void OnLevelChangedCallback(int32 NewLevel, bool bLevelUp);

	UFUNCTION()
	void OnGetItemFromTreasureBoxCallback(const FGameplayTag& ItemTag, int32 InAmount, bool bLoadData);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Widget Data")
	TObjectPtr<UDataTable> MessageWidgetRowDataTable;


private:

	float CalculateXPPercent(int32 NewXP);

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	
};

template<typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}

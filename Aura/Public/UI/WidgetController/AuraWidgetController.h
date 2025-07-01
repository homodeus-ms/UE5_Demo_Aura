// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoDelegate, const struct FAuraAbilityInfo&, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatChangeDelegate, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLevelChangeDelegate, int32, NewValue, bool, bLevelUp);

class UAbilitySystemComponent;
class UAttributeSet;
class APlayerController;
class APlayerState;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* InASC, UAttributeSet* AS)
		: PlayerController(PC), PlayerState(PS), ASC(InASC), AttributeSet(AS) { }
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> ASC = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;

};
/**
 * 
 */
UCLASS(Abstract)
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& Params);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() PURE_VIRTUAL(UAuraWidgetController::BroadcastInitialValues, );
	virtual void BindCallbacksToDependencies() PURE_VIRTUAL(UAuraWidgetController::BroadcastInitialValues, );
	
	class AAuraPlayerState* GetAuraPlayerState();
	class UAuraAbilitySystemComponent* GetAuraASC();
	class UAuraAttributeSet* GetAuraAttributeSet();

	UPROPERTY(BlueprintAssignable, Category = "GAS|Message")
	FAbilityInfoDelegate AbilityInfoDelegate;

	UFUNCTION()
	virtual void BroadcastAbilityInfoChanged();

	UFUNCTION()
	void OnAbilityEquipChanged(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& InputTag, const FGameplayTag& PrevBoxInputTagOrEmpty);

protected:

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<class APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<class APlayerState> PlayerState;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<class UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<class UAbilityInfo> AbilityInfo;

	// Cached Aura Version

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<class AAuraPlayerState> AuraPlayerState = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<class UAuraAbilitySystemComponent> AuraASC = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<class UAuraAttributeSet> AuraAttributeSet = nullptr;
};

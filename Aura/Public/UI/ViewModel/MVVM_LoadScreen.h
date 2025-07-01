// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeleteButtonPressedDelegate, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectButtonPressedDelegate, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFocusedSlotChangedDelegate);
/**
 * 
 */
class UMVVM_LoadSlots;

UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:

	/*void InitLoadSlotVMs();

	void LoadSlotData();

	UFUNCTION(BlueprintCallable)
	void SetSelectedSlotIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	void NotifySlotFocused(int32 SlotIndex, bool bNotifyAll = false);

	UFUNCTION(BlueprintCallable)
	UMVVM_LoadSlots* GetLoadSlotVMByIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	void StartButtonPressed();


	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void EnterNameButtonPressed(int32 SlotIndex, const FString& EnteredName);

	UFUNCTION(BlueprintCallable)
	void SelectButtonPressed(int32 SlotIndex);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDeleteButtonPressedDelegate OnDeleteButtonPressed;

	UPROPERTY(BlueprintAssignable)
	FSelectButtonPressedDelegate OnSelectButtonPressedDelegate;

	UPROPERTY(BlueprintAssignable)
	FFocusedSlotChangedDelegate OnFocusedSlotChangedDelegate;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlots> LoadSlotVMClass;

protected:
	UPROPERTY()
	TArray<TObjectPtr<UMVVM_LoadSlots>> LoadSlotVMs;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlots> LoadSlotVM_0;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlots> LoadSlotVM_1;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlots> LoadSlotVM_2;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	int32 SelectedSlotIndex = -1;*/
};

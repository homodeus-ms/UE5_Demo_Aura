// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "SaveGame/LoadScreenSaveGame.h"
#include "MVVM_LoadSlots.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSwicherIndexChangedDelegate, int32, SlotIndex);

/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadSlots : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:

	/*UFUNCTION(BlueprintCallable)
	void ActivateSwitcher();

	UFUNCTION(BlueprintCallable)
	void DeleteSelectedSlotSavedData(int32 SlotIndex);

	FString GetLoadSlotName() const { return LoadSlotName; }
	FString GetPlayerName() const { return PlayerName; }
	FString GetMapName() const { return MapName; }
	int32 GetPlayerLevel() const { return PlayerLevel; }

	void SetLoadSlotName(FString InLoadSlotName);
	void SetPlayerName(FString InPlayerName);
	void SetPlayerLevel(int32 InPlayerLevel);
	void SetMapName(FString InMapName);
	void SetSlotStatus(TEnumAsByte<ESlotStatus> Status);

	UPROPERTY(BlueprintAssignable)
	FSwicherIndexChangedDelegate OnSwitcherIndexChangedDelegate;

	UPROPERTY()
	TEnumAsByte<ESlotStatus> SlotStatus = ESlotStatus::Empty;

	FName PlayerStartTag = FName();
	FName MapAssetName = FName();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	FString LoadSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 PlayerLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString MapName;*/
};

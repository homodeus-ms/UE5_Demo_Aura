// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadSlots.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

//void UMVVM_LoadSlots::ActivateSwitcher()
//{
//	const int8 ActiveSlotIndex = SlotStatus.GetValue();
//	OnSwitcherIndexChangedDelegate.Broadcast(ActiveSlotIndex);
//}
//
//void UMVVM_LoadSlots::DeleteSelectedSlotSavedData(int32 SlotIndex)
//{
//	check(SlotIndex != -1);
//	AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
//
//	AuraGM->DeleteSavedData(LoadSlotName, SlotIndex);
//	SetSlotStatus(ESlotStatus::Empty);
//	ActivateSwitcher();
//}
//
//
//void UMVVM_LoadSlots::SetLoadSlotName(FString InLoadSlotName)
//{
//	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
//}
//
//void UMVVM_LoadSlots::SetPlayerName(FString InPlayerName)
//{
//	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
//}
//
//void UMVVM_LoadSlots::SetPlayerLevel(int32 InPlayerLevel)
//{
//	UE_MVVM_SET_PROPERTY_VALUE(PlayerLevel, InPlayerLevel);
//}
//
//void UMVVM_LoadSlots::SetMapName(FString InMapName)
//{
//	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
//}
//
//void UMVVM_LoadSlots::SetSlotStatus(TEnumAsByte<ESlotStatus> Status)
//{
//	SlotStatus = Status;
//}

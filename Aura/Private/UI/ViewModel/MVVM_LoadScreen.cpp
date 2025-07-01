// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/ViewModel/MVVM_LoadSlots.h"
#include "AuraLog.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Game/AuraGameInstance.h"

//void UMVVM_LoadScreen::InitLoadSlotVMs()
//{
//	LoadSlotVM_0 = NewObject<UMVVM_LoadSlots>(this, LoadSlotVMClass);
//	LoadSlotVMs.Add(LoadSlotVM_0);
//	LoadSlotVM_0->SetLoadSlotName(TEXT("LoadSlot_0"));
//
//	LoadSlotVM_1 = NewObject<UMVVM_LoadSlots>(this, LoadSlotVMClass);
//	LoadSlotVMs.Add(LoadSlotVM_1);
//	LoadSlotVM_1->SetLoadSlotName(TEXT("LoadSlot_1"));
//
//	LoadSlotVM_2 = NewObject<UMVVM_LoadSlots>(this, LoadSlotVMClass);
//	LoadSlotVMs.Add(LoadSlotVM_2);
//	LoadSlotVM_2->SetLoadSlotName(TEXT("LoadSlot_2"));
//
//}
//
//void UMVVM_LoadScreen::LoadSlotData()
//{
//	
//}
//
//void UMVVM_LoadScreen::SetSelectedSlotIndex(int32 Index)
//{
//	SelectedSlotIndex = Index;
//}
//
//void UMVVM_LoadScreen::NotifySlotFocused(int32 SlotIndex, bool bNotifyAll)
//{
//	if (bNotifyAll)
//	{
//		OnFocusedSlotChangedDelegate.Broadcast();
//		return;
//	}
//
//	if (SelectedSlotIndex == -1)
//		return;
//
//	if (SelectedSlotIndex != SlotIndex)
//	{
//		SelectedSlotIndex = -1;
//		OnFocusedSlotChangedDelegate.Broadcast();
//	}
//}
//
//
//UMVVM_LoadSlots* UMVVM_LoadScreen::GetLoadSlotVMByIndex(int32 Index)
//{
//	check(LoadSlotVMs.Num() == 3);
//	check(LoadSlotVMs.IsValidIndex(Index));
//
//	return LoadSlotVMs[Index];
//}
//
//void UMVVM_LoadScreen::StartButtonPressed()
//{
//	if (!LoadSlotVMs.IsValidIndex(SelectedSlotIndex))
//	{
//		UE_LOG(Aura, Warning, TEXT("SelectedSlotIndex Is Not Valid"));
//		return;
//	}
//	AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
//	UAuraGameInstance* AuraGI = AuraGM->GetAuraGameInstance();
//	UMVVM_LoadSlots* LoadSlotVM = LoadSlotVMs[SelectedSlotIndex];
//
//	AuraGI->SetInfo(LoadSlotVM->PlayerStartTag, LoadSlotVM->GetLoadSlotName(), SelectedSlotIndex);
//
//	//AuraGM->TravelToMap(LoadSlotVM);
//}
//
//void UMVVM_LoadScreen::NewGameButtonPressed(int32 SlotIndex)
//{
//	NotifySlotFocused(SlotIndex);
//	UMVVM_LoadSlots* LoadSlotVM = GetLoadSlotVMByIndex(SlotIndex);
//	LoadSlotVM->SlotStatus = ESlotStatus::EnterName;
//	LoadSlotVM->OnSwitcherIndexChangedDelegate.Broadcast(ESlotStatus::EnterName);
//	
//
//}
//
//void UMVVM_LoadScreen::EnterNameButtonPressed(int32 SlotIndex, const FString& EnteredName)
//{
//	NotifySlotFocused(SlotIndex);
//
//	AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
//
//	UMVVM_LoadSlots* LoadSlotVM = GetLoadSlotVMByIndex(SlotIndex);
//	LoadSlotVM->SlotStatus = ESlotStatus::ShowInfoAndPlay;
//	LoadSlotVM->SetPlayerName(EnteredName);
//	LoadSlotVM->SetPlayerLevel(1);
//	LoadSlotVM->SetMapName(AuraGM->DefaultMapName);
//	LoadSlotVM->MapAssetName = AuraGM->GetMapAssetName(AuraGM->DefaultMap);
//	LoadSlotVM->PlayerStartTag = AuraGM->DefaultPlayerStartTag;
//
//	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(AuraGM->GetGameInstance());
//	AuraGI->SetInfo(LoadSlotVM->PlayerStartTag, LoadSlotVM->GetPlayerName(), SelectedSlotIndex);
//
//	AuraGM->SaveSlotDataByEnterNameButton(LoadSlotVM, SlotIndex);
//
//	LoadSlotVM->OnSwitcherIndexChangedDelegate.Broadcast(ESlotStatus::ShowInfoAndPlay);
//}
//
//void UMVVM_LoadScreen::SelectButtonPressed(int32 SlotIndex)
//{
//	NotifySlotFocused(SlotIndex);
//
//	SetSelectedSlotIndex(SlotIndex);
//	
//	OnSelectButtonPressedDelegate.Broadcast(SlotIndex);
//
//}

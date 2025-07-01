// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlots.h"
#include "SaveGame/LoadScreenSaveGame.h"
#include "Save/AuraSaveData.h"
#include "GameFramework/PlayerStart.h"
#include "Game/AuraGameInstance.h"
#include "AuraLog.h"
#include "Save/AuraSaveData.h"
#include "DedicatedServer/Public/Common/DedicatedServer.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h" 
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "DedicatedServer/Public/Interfaces/SaveManageInterface.h"
#include "AuraGameplayTags.h"
#include "ActorFactoryMap.h"

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Maps.Add(DefaultMapName, DefaultMap);

	FactoryMap = NewObject<UActorFactoryMap>(this, ActorFactoryMapClass);
	
	FactoryMap->RegisterMap();
}

void AAuraGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(Aura, Log, TEXT("AAuraGameModeBase::PostLogin()"));

	FAuraGameplayTags::InitAuraGameplayTags();

	AAuraPlayerController* AuraPC = Cast<AAuraPlayerController>(NewPlayer);
	if (!AuraPC)
	{
		UE_LOG(Aura, Log, TEXT("PlayerController Is NOT Valid!, %s"), *FString(__FUNCTION__));
		return;
	}

	AuraPC->OnHudInitialized.BindUObject(AuraPC, &ADS_PlayerController::LoadAndMoveToSavedLocation);

	AAuraPlayerState* AuraPS = AuraPC->GetPlayerState<AAuraPlayerState>();
	if (!AuraPS)
	{
		UE_LOG(Aura, Log, TEXT("PlayerState Is NOT Valid!, %s"), *FString(__FUNCTION__));
		return;
	}

	bool IsNewPlayer = AuraPC->IsNewPlayer();
	bool IsRespawn = AuraPC->IsRespawn();

	if (IsNewPlayer && !IsRespawn)
	{
		UE_LOG(Aura, Log, TEXT("----- New Player, Call AuraPS->LoadNewPlayerDatas()"));
		AuraPS->LoadNewPlayerDatas();
	}
	else
	{
		if (ISaveManageInterface* SaveManageInterface = AuraPS->GetSaveManageInterface())
		{
			const FString& Sub = AuraPS->GetSub();
			
			SaveManageInterface->LoadGame(Sub, FOnRetrieveInfosDelegate::CreateUObject(AuraPS, &AAuraPlayerState::OnRetrieveData));
		}
	}
	
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		if (It->PlayerStartTag == DefaultPlayerStartTag)
		{
			return *It;
		}
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

UAuraGameInstance* AAuraGameModeBase::GetAuraGameInstance()
{
	return CastChecked<UAuraGameInstance>(GetGameInstance());;
}

//FString AAuraGameModeBase::GetMapNameByMapAsset(TSoftObjectPtr<UWorld> MapAsset)
//{
//	for (const auto& Pair : Maps)
//	{
//		const FString PairAssetName = Pair.Value.ToSoftObjectPath().GetAssetName();
//		const FString InAssetName = MapAsset.ToSoftObjectPath().GetAssetName();
//
//		if (PairAssetName == InAssetName)
//		{
//			return Pair.Key;
//		}
//	}
//	return FString();
//}
//
//FName AAuraGameModeBase::GetMapAssetName(TSoftObjectPtr<UWorld> MapAsset)
//{
//	if (MapAsset.ToSoftObjectPath().IsValid())
//	{
//		return FName(*MapAsset.ToSoftObjectPath().GetAssetName());
//	}
//	return FName();
//}
//
//void AAuraGameModeBase::SaveMapNamesToSaveGameInstance(TSoftObjectPtr<UWorld> MapAsset)
//{
//	
//}

//UAuraSaveData* AAuraGameModeBase::GetSavedData()
//{
//	UDS_GameInstanceSubsystem* Subsystem = GetDSGameInstanceSubsystem();
//	if (!Subsystem)
//	{
//		UE_LOG(LogAuraDS, Warning, TEXT("UDS_GameInstanceSubsystem Is NOT Valid! %s"), *FString(__FUNCTION__));
//		return nullptr;
//	}
//
//	UAuraSaveData* SaveData = Subsystem->GetSaveData();
//	if (!SaveData)
//	{
//		UE_LOG(LogAuraDS, Warning, TEXT("SaveData From GetSaveData Is NOT Valid! %s"), *FString(__FUNCTION__));
//		return nullptr;
//	}
//
//	return SaveData;
//}

//ULoadScreenSaveGame* AAuraGameModeBase::FindSaveData()
//{
//	
//	return nullptr;
//}
//
//void AAuraGameModeBase::SaveSlotDataByEnterNameButton(UMVVM_LoadSlots* LoadSlotVM, int32 SlotIndex)
//{
//	const FString& SlotName = LoadSlotVM->GetLoadSlotName();
//
//	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
//	{
//		DeleteSavedData(SlotName, SlotIndex);
//	}
//
//	USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
//	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGame);
//
//	LoadScreenSaveGame->SlotName = LoadSlotVM->GetLoadSlotName();
//	LoadScreenSaveGame->SlotIndex = SlotIndex;
//	LoadScreenSaveGame->PlayerName = LoadSlotVM->GetPlayerName();
//	LoadScreenSaveGame->MapName = LoadSlotVM->GetMapName();
//	LoadScreenSaveGame->MapAssetName = LoadSlotVM->MapAssetName;
//	LoadScreenSaveGame->SlotStatus = LoadSlotVM->SlotStatus;
//	LoadScreenSaveGame->PlayerStartTag = GetAuraGameInstance()->PlayerStartTag;
//
//	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, SlotName, SlotIndex);
//}
//
//void AAuraGameModeBase::DeleteSavedData(const FString& SlotName, int32 SlotIndex)
//{
//	UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
//}


void AAuraGameModeBase::PlayerDied(AActor* DeadCharacter)
{
	UE_LOG(Aura, Log, TEXT("AAuraGameModeBase::PlayerDied()"));

	
}



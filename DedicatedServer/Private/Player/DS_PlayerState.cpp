// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DS_PlayerState.h"
#include "Common/DedicatedServer.h"
#include "Save/AuraSaveData.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "Interfaces/SaveManageInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"



void ADS_PlayerState::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogAuraDS, Log, TEXT("ADS_PlayerState::BeginPlay()"));
    
    if (!HasAuthority())
        return;

    if (UDS_GameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UDS_GameInstanceSubsystem>())
    {
        if (ISaveManageInterface* SaveInterface = Subsystem->GetSaveManageInterface())
        {
            SaveManageInterface = SaveInterface->_getUObject();
        }
    }
}

void ADS_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ADS_PlayerState, LastMapName);
    DOREPLIFETIME(ADS_PlayerState, LastPlayerStartName);
    DOREPLIFETIME(ADS_PlayerState, UserName);
}


const FString& ADS_PlayerState::GetLocalPlayerUserName() const
{
    return UserName;
}

void ADS_PlayerState::SetUserInfo(const FString& InUserName, const FString& InSub)
{
    UserName = InUserName;
    Sub = InSub;

    UE_LOG(LogAuraDS, Log, TEXT("PlayerState, UserName: %s, Sub: %s"), *UserName, *Sub);
}

void ADS_PlayerState::SavePlayersLastMapInfoToGISubsystem(const FString& MapName, const FString& PlayerStartName)
{
    // Save Info Using PlayerState's Replicate System
    if (HasAuthority())
    {
        LastMapName = MapName;
        LastPlayerStartName = PlayerStartName;
    }
}

UAuraSaveData* ADS_PlayerState::GetSaveData()
{
    if (!SaveData)
    {
        SaveData = NewObject<UAuraSaveData>(this);
    }
    return SaveData;
}

void ADS_PlayerState::ResetSaveData()
{
    SaveData = nullptr;
}

ISaveManageInterface* ADS_PlayerState::GetSaveManageInterface()
{
    check(SaveManageInterface);
    return SaveManageInterface.GetInterface();
}

void ADS_PlayerState::OnRep_LastMapName()
{
    if (UDS_GameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UDS_GameInstanceSubsystem>())
    {
        Subsystem->KeepLastMapName(LastMapName);
    }
}

void ADS_PlayerState::OnRep_LastPlayerStartName()
{
    if (UDS_GameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UDS_GameInstanceSubsystem>())
    {
        Subsystem->KeepLastPlayerStartName(LastPlayerStartName);
    }
}

void ADS_PlayerState::OnRep_UserName()
{
    UE_LOG(LogAuraDS, Log, TEXT("ADS_PlayerState::OnRep_UserName()"));
}

//const FString& ADS_PlayerState::GetLastMapName() const
//{
//    if (!SaveData)
//    {
//        return DefaultMapName;
//    }
//    return SaveData->PlayerData.MapName;
//}
//
//void ADS_PlayerState::SetLastMapName(const FString& MapName)
//{
//    GetSaveData()->PlayerData.MapName = MapName;
//}



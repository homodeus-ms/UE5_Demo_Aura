// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "Common/DedicatedServer.h"

UDS_GameInstanceSubsystem* ADS_GameModeBase::GetDSGameInstanceSubsystem() const
{
    UGameInstance* GameInstance = GetGameInstance();
    if (IsValid(GameInstance))
    {
        UDS_GameInstanceSubsystem* Subsystem = GameInstance->GetSubsystem<UDS_GameInstanceSubsystem>();
        if (Subsystem)
            return Subsystem;
    }

    UE_LOG(LogAuraDS, Warning, TEXT("UDS_GameInstanceSubsystem Is NOT Valid!"));

    return nullptr;
}

ISaveManageInterface* ADS_GameModeBase::GetSaveManageInterface()
{
    UGameInstance* GameInstance = GetGameInstance();
    check(GameInstance);

    UDS_GameInstanceSubsystem* Subsystem = GameInstance->GetSubsystem<UDS_GameInstanceSubsystem>();
    check(Subsystem);

    return Subsystem->GetSaveManageInterface();
}

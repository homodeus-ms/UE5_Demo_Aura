// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_GameInstanceSubsystem.h"
#include "Common/DedicatedServer.h"
#include "HTTP/SaveManager.h"
#include "Save/AuraSaveData.h"
#include "Interfaces/SaveManageInterface.h"
#include "Interfaces/GameSessionManageInterface.h"
#include "Kismet/GameplayStatics.h"
#include "HTTP/GameSessionManager.h"


UDS_GameInstanceSubsystem::UDS_GameInstanceSubsystem()
{
    static ConstructorHelpers::FClassFinder<USaveManager> SaveManagerPath(TEXT("/Game/DedicatedServer/Manager/BP_SaveManager"));

    if (SaveManagerPath.Succeeded())
    {
        SaveManagerClass = SaveManagerPath.Class;
    }
    else
    {
        UE_LOG(LogAuraDS, Error, TEXT("SaveManagerPath NOT Valid!"));
    }

    static ConstructorHelpers::FClassFinder<UGameSessionManager> SessionManagerPath(TEXT("/Game/DedicatedServer/Manager/BP_GameSessionManager"));

    if (SessionManagerPath.Succeeded())
    {
        GameSessionManagerClass = SessionManagerPath.Class;
    }
    else
    {
        UE_LOG(LogAuraDS, Error, TEXT("GameSessionManagerPath NOT Valid!"));
    }
}

void UDS_GameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (SaveManagerClass)
    {
        SaveManager = NewObject<USaveManager>(this, SaveManagerClass);
        check(SaveManager);
        SaveManager->SetGameInstanceSubsystem(this);
    }
    else
    {
        UE_LOG(LogAuraDS, Error, TEXT("SaveManager NOT VALID!"));
    }

    if (GameSessionManagerClass)
    {
        GameSessionManager = NewObject<UGameSessionManager>(this, GameSessionManagerClass);
        check(GameSessionManager);
        GameSessionManager->SetGameInstanceSubsystem(this);
    }
    else
    {
        UE_LOG(LogAuraDS, Error, TEXT("GameSessionManager NOT VALID!"));
    }
}

void UDS_GameInstanceSubsystem::InitGameLift(const FServerParameters& ServerParameters)
{
    if (bGameLiftInitialized)
        return;

#if WITH_GAMELIFT

    UE_LOG(LogAuraDS, Log, TEXT("Initialize GameLift Server"));
    FGameLiftServerSDKModule* GameLiftSDKModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

    // 1. For Anywhere fleets : InitSDK(ServerParams);
    // 2. For EC2 fleets : Just Call InitSDK();
    GameLiftSDKModule->InitSDK(ServerParameters);

    SetProcessParameters(GameLiftSDKModule);

    UE_LOG(LogAuraDS, Log, TEXT("Calling Process Ready"));

    GameLiftSDKModule->ProcessReady(ProcessParams);

#endif

    bGameLiftInitialized = true;
}

USaveManager* UDS_GameInstanceSubsystem::GetSaveManager() const
{
    check(SaveManager);
    return SaveManager;
}

ISaveManageInterface* UDS_GameInstanceSubsystem::GetSaveManageInterface()
{
    return CastChecked<ISaveManageInterface>(SaveManager);
}

UGameSessionManager* UDS_GameInstanceSubsystem::GetGameSessionManager() const
{
    check(GameSessionManager);
    return GameSessionManager;
}

IGameSessionManageInterface* UDS_GameInstanceSubsystem::GetGameSessionManageInterface()
{
    return CastChecked<IGameSessionManageInterface>(GameSessionManager);
}

void UDS_GameInstanceSubsystem::KeepLastMapName(const FString& MapName)
{
    UE_LOG(LogAuraDS, Log, TEXT("UDS_GameInstanceSubsystem::KeepLastMapName, %s"), *MapName);
    LastMapName = MapName;
}

void UDS_GameInstanceSubsystem::KeepLastPlayerStartName(const FString& PlayerStartName)
{
    LastPlayerStartName = PlayerStartName;
}

void UDS_GameInstanceSubsystem::SetProcessParameters(FGameLiftServerSDKModule* GameLiftSDKModule)
{
    auto OnGameSession = [=](Aws::GameLift::Server::Model::GameSession InGameSession)
        {
            FString GameSessionID = FString(InGameSession.GetGameSessionId());
            UE_LOG(LogAuraDS, Log, TEXT("GameSession Initializing: %s"), *GameSessionID);
            GameLiftSDKModule->ActivateGameSession();
        };
    ProcessParams.OnStartGameSession.BindLambda(OnGameSession);

    auto OnGameEnd = [=]()
        {
            UE_LOG(LogAuraDS, Log, TEXT("Game Server process is terminated"));
            GameLiftSDKModule->ProcessEnding();
        };
    ProcessParams.OnTerminate.BindLambda(OnGameEnd);

    auto OnHealthCheck = [=]()
        {
            UE_LOG(LogAuraDS, Log, TEXT("Health Checking"));
            return true;
        };
    ProcessParams.OnHealthCheck.BindLambda(OnHealthCheck);

    ProcessParams.port = GetPortNumber();

    TArray<FString> LogFiles;
    LogFiles.Add(TEXT("Aura/Saved/Logs/AuraLog.log"));
    ProcessParams.logParameters = LogFiles;
}

int32 UDS_GameInstanceSubsystem::GetPortNumber()
{
    int32 Port = FURL::UrlConfig.DefaultPort;    // DefaultPort = 7777

    TArray<FString> CommandLineTokens;
    TArray<FString> CommandLineSwitches;
    FCommandLine::Parse(FCommandLine::Get(), CommandLineTokens, CommandLineSwitches);

    for (const FString& Switch : CommandLineSwitches)
    {
        FString Key;
        FString Value;
        if (Switch.Split("=", &Key, &Value))
        {
            if (Key.Equals(TEXT("port"), ESearchCase::IgnoreCase) && Value.IsNumeric())
            {
                LexFromString(Port, *Value);
                break;
            }
        }
    }

    return Port;
}





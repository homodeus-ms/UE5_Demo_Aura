// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_LobbyGameMode.h"
#include "Common/DedicatedServer.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DS_PlayerController.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"

ADS_LobbyGameMode::ADS_LobbyGameMode()
{
    bUseSeamlessTravel = true;
}

void ADS_LobbyGameMode::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogAuraDS, Log, TEXT("ADS_LobbyGameMode::BeginPlay()!"));

    InitGameLift();
}

void ADS_LobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

    const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
    const FString UserName = UGameplayStatics::ParseOption(Options, TEXT("UserName"));

    if (!TryAcceptPlayerSession(PlayerSessionId, UserName, ErrorMessage))
        ReturnToPortal();
}

FString ADS_LobbyGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
    const FString InitString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options);

    const FString& PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
    const FString& UserName = UGameplayStatics::ParseOption(Options, TEXT("UserName"));

    ADS_PlayerController* DSPlayerController = Cast<ADS_PlayerController>(NewPlayerController);
    if (IsValid(DSPlayerController))
    {
        DSPlayerController->SetUserInfos(PlayerSessionId, UserName);
    }

    return InitString;
}

void ADS_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
}


bool ADS_LobbyGameMode::TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& PlayerId, FString& OUTErrorMessage)
{
    if (PlayerSessionId.IsEmpty() || PlayerId.IsEmpty())
    {
        OUTErrorMessage = TEXT("PlayerSessionId and(or) UserName Is Not Vaild");
        return false;
    }

#if WITH_GAMELIFT

    namespace AWS_SERVER = Aws::GameLift::Server;
    namespace AWS_MODEL = AWS_SERVER::Model;

    AWS_MODEL::DescribePlayerSessionsRequest PlayerSessionRequest;
    PlayerSessionRequest.SetPlayerSessionId(TCHAR_TO_ANSI(*PlayerSessionId));

    const auto& DescribeOutcome = AWS_SERVER::DescribePlayerSessions(PlayerSessionRequest);
    if (!DescribeOutcome.IsSuccess())
    {
        OUTErrorMessage = TEXT("Describe Player Session Failed");
        return false;
    }

    const auto& DescribeResult = DescribeOutcome.GetResult();
    int32 OUTCount = 0;
    const AWS_MODEL::PlayerSession* PlayerSessions = DescribeResult.GetPlayerSessions(OUTCount);
    if (PlayerSessions == nullptr || OUTCount == 0)
    {
        OUTErrorMessage = TEXT("Get Player Session Failed");
        return false;
    }

    const AWS_MODEL::PlayerSession* PlayerSession = nullptr;

    for (int32 i = 0; i < OUTCount; ++i)
    {
        if (PlayerId.Equals(PlayerSessions[i].GetPlayerId()))
        {
            PlayerSession = &PlayerSessions[i];
            break;
        }
    }

    if (PlayerSession == nullptr)
    {
        OUTErrorMessage = FString::Printf(TEXT("Not Found Player Session"));
        return false;
    }

    if ((*PlayerSession).GetStatus() != AWS_MODEL::PlayerSessionStatus::RESERVED)
    {
        OUTErrorMessage = FString::Printf(TEXT("Session For %s Not Reserved"), *PlayerId);
        return false;
    }

    const auto& AcceptOutcome = AWS_SERVER::AcceptPlayerSession(TCHAR_TO_ANSI(*PlayerSessionId));
    if (!AcceptOutcome.IsSuccess())
    {
        OUTErrorMessage = FString::Printf(TEXT("%s Session Not Accepted"), *PlayerId);
        return false;
    }

#endif

    return true;
}

void ADS_LobbyGameMode::ReturnToPortal()
{
    UE_LOG(LogAuraDS, Error, TEXT("ReturnToPortal, TryAcceptPlayerSession Failed!"));
}

FName ADS_LobbyGameMode::GetPlayerStartName()
{
    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
    {
        APlayerStart* PlayerStart = *It;
        if (PlayerStart->GetLevel()->GetOuter()->GetFName() == DefaultLevelName)
            return PlayerStart->GetFName();
    }

    return FName();
}


void ADS_LobbyGameMode::InitGameLift()
{
    DSGameInstanceSubsystem = GetDSGameInstanceSubsystem();
    if (IsValid(DSGameInstanceSubsystem))
    {
        UE_LOG(LogAuraDS, Log, TEXT("ADS_LobbyGameMode::InitGameLift()!"));
        FServerParameters ServerParams;
        SetServerParameteres(ServerParams);
        DSGameInstanceSubsystem->InitGameLift(ServerParams);
    }
    else
    {
        UE_LOG(LogAuraDS, Log, TEXT("DSGameInstanceSubsystem is NOT Valid!"));
    }
}

void ADS_LobbyGameMode::SetServerParameteres(FServerParameters& OUTServerParams)
{
    // AuthToken returned from the "aws gamelift get-compute-auth-token" API. 
    // Note this will expire and require a new call to the API after 15 minutes.
    if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OUTServerParams.m_authToken))
    {
        UE_LOG(LogAuraDS, Log, TEXT("AUTH_TOKEN: %s"), *OUTServerParams.m_authToken);
    }

    //The Host/compute-name of the Amazon GameLift Servers Anywhere instance.
    if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), OUTServerParams.m_hostId))
    {
        UE_LOG(LogAuraDS, Log, TEXT("HOST_ID: %s"), *OUTServerParams.m_hostId);
    }

    //The Anywhere Fleet ID.
    if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), OUTServerParams.m_fleetId))
    {
        UE_LOG(LogAuraDS, Log, TEXT("FLEET_ID: %s"), *OUTServerParams.m_fleetId);
    }

    //The WebSocket URL (GameLiftServiceSdkEndpoint).
    if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), OUTServerParams.m_webSocketUrl))
    {
        UE_LOG(LogAuraDS, Log, TEXT("WEBSOCKET_URL: %s"), *OUTServerParams.m_webSocketUrl);
    }

    OUTServerParams.m_processId = FString::Printf(TEXT("%d"), GetCurrentProcessId());
    UE_LOG(LogAuraDS, Log, TEXT("PID: %s"), *OUTServerParams.m_processId);
}

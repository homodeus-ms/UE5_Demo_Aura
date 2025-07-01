// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_AuraGameMode.h"
#include "Common/DedicatedServer.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DS_PlayerController.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Player/DS_PlayerState.h"

ADS_AuraGameMode::ADS_AuraGameMode()
{
    bUseSeamlessTravel = true;
}

void ADS_AuraGameMode::BeginPlay()
{
	Super::BeginPlay();

    UE_LOG(LogAuraDS, Log, TEXT("InitGameLift!"));

    InitGameLift();
}

void ADS_AuraGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

    UE_LOG(LogAuraDS, Log, TEXT("ADS_AuraGameMode::PreLogin()!"));

    const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
    const FString UserName = UGameplayStatics::ParseOption(Options, TEXT("UserName"));

    if (!TryAcceptPlayerSession(PlayerSessionId, UserName, ErrorMessage))
    {
        UE_LOG(LogAuraDS, Error, TEXT("Player Session Denied!"));
        ReturnToPortal();
    }
}

FString ADS_AuraGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
    UE_LOG(LogAuraDS, Log, TEXT("ADS_AuraGameMode::InitNewPlayer()!"));

    const FString InitString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options);

    UE_LOG(LogAuraDS, Log, TEXT("InitString: %s"), *InitString);

    const FString& PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
    const FString& UserName = UGameplayStatics::ParseOption(Options, TEXT("UserName"));
    const FString& Sub = UGameplayStatics::ParseOption(Options, TEXT("Sub"));
    const FName LastMapName(UGameplayStatics::ParseOption(Options, TEXT("LastMapName")));
    const FName LastPlayerStartName(UGameplayStatics::ParseOption(Options, TEXT("LastPlayerStartName")));
    const FName& PersistentMapName = GetWorld()->PersistentLevel->GetOuter()->GetFName();
    const bool IsNewPlayer = UGameplayStatics::ParseOption(Options, TEXT("IsNewPlayer")) == TEXT("1");
    const bool IsRespawn = UGameplayStatics::ParseOption(Options, TEXT("IsRespawn")) == TEXT("1");

    UE_LOG(LogAuraDS, Log, TEXT("--- DS_AuraGameMode ---"));
    UE_LOG(LogAuraDS, Log, TEXT("LastMapName: %s"), *LastMapName.ToString());
    UE_LOG(LogAuraDS, Log, TEXT("LastPlayerStart: %s"), *LastPlayerStartName.ToString());
    
    
    ADS_PlayerController* DSPlayerController = Cast<ADS_PlayerController>(NewPlayerController);
    if (IsValid(DSPlayerController))
    {
        DSPlayerController->SetUserInfos(PlayerSessionId, UserName, Sub);
        DSPlayerController->SetSavedUserLocation(LastMapName, LastPlayerStartName);
        DSPlayerController->SetIsNewPlayer(IsNewPlayer);
        DSPlayerController->SetIsRespawn(IsRespawn);
    }

    return InitString;
}

void ADS_AuraGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    UE_LOG(LogAuraDS, Log, TEXT("ADS_AuraGameMode::PostLogin"));
}

void ADS_AuraGameMode::Logout(AController* Existing)
{
    Super::Logout(Existing);

    RemovePlayerSession(Existing);
}


bool ADS_AuraGameMode::TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& PlayerId, FString& OUTErrorMessage)
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

void ADS_AuraGameMode::ReturnToPortal()
{
    UE_LOG(LogAuraDS, Error, TEXT("ReturnToPortal, TryAcceptPlayerSession Failed!"));
}

FName ADS_AuraGameMode::GetPlayerStartName()
{
    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
    {
        APlayerStart* PlayerStart = *It;
        if (PlayerStart->GetLevel()->GetOuter()->GetFName() == DefaultLevelName)
            return PlayerStart->GetFName();
    }

    return FName();
}


void ADS_AuraGameMode::InitGameLift()
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

void ADS_AuraGameMode::SetServerParameteres(FServerParameters& OUTServerParams)
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

void ADS_AuraGameMode::RemovePlayerSession(AController* Existing)
{
    ADS_PlayerController* DSPlayerController = Cast<ADS_PlayerController>(Existing);
    if (!IsValid(DSPlayerController))
        return;

    const FString& LeavingUserName = DSPlayerController->GetPlayerName();
    DSPlayerController->SendMessageToAllClient(TEXT(""), FString::Printf(TEXT("--- %s 가 게임에서 나갔습니다!"), *LeavingUserName));
     
#if WITH_GAMELIFT
    const FString& PlayerSessionId = DSPlayerController->GetPlayerSessionId();
    if (!PlayerSessionId.IsEmpty())
    {
        UE_LOG(LogAuraDS, Log, TEXT("Remove Player Session: %s"), *DSPlayerController->GetPlayerName());
        Aws::GameLift::Server::RemovePlayerSession(TCHAR_TO_ANSI(*PlayerSessionId));
    }

#endif
}


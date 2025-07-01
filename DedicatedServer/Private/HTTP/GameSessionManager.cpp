// Fill out your copyright notice in the Description page of Project Settings.


#include "HTTP/GameSessionManager.h"
#include "Game/DS_GameModeBase.h"
#include "Common/DedicatedServer.h"
#include "HttpModule.h"
#include "DedicatedServer/DedicatedServerTags.h"
#include "JsonObjectConverter.h"
#include "Save/AuraSaveData.h"
#include "Player/DS_LocalPlayerSubsystem.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UGameSessionManager::JoinGameSession()
{
	UE_LOG(LogAuraDS, Log, TEXT("JoinSession!"));

	JoinStatusMessageDelegate.Broadcast(TEXT("게임 세션을 찾고 있습니다..."), false);
	check(APIData);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::JoinGameSession_Response);

	FAPIRequestSendData SendData(DedicatedServerTags::GameSessionAPI::FindOrCreateGameSession, TEXT("POST"));

	if (UDS_LocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem())
	{
		const FString& AccessToken = LocalPlayerSubsystem->GetAuthResult().AccessToken;
		Request->SetHeader(TEXT("Authorization"), AccessToken);
	}

	SendHttpRequest(Request, SendData);
}

void UGameSessionManager::TryCreatePlayerSession(const FString& PlayerId, const FString& GameSessionId)
{
	JoinStatusMessageDelegate.Broadcast(TEXT("플레이어 세션을 만들고 접속중입니다..."), false);
	UE_LOG(LogAuraDS, Log, TEXT("UGameSessionManager::TryCreatePlayerSession()"));
	check(APIData);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::CreatePlayerSession_Response);

	FAPIRequestSendData SendData(DedicatedServerTags::GameSessionAPI::CreatePlayerSession, TEXT("POST"));
	SendData.Params.Add(TEXT("playerId"), PlayerId);
	SendData.Params.Add(TEXT("gameSessionId"), GameSessionId);

	SendHttpRequest(Request, SendData);
}

void UGameSessionManager::JoinGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogAuraDS, Log, TEXT("UGameSessionManager::JoinGameSession_Response()"));

	if (!bWasSuccessful)
	{
		JoinStatusMessageDelegate.Broadcast(HTTPStatusMessage::SomethingWrong, true);
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			JoinStatusMessageDelegate.Broadcast(HTTPStatusMessage::SomethingWrong, true);
			
			return;
		}

		// Successful Response
		JoinStatusMessageDelegate.Broadcast(TEXT("게임 세션을 찾았습니다..."), false);
		UE_LOG(LogAuraDS, Log, TEXT("UGameSessionManager::JoinGameSession_Response, Successful!"));

		FDSGameSession GameSession;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &GameSession);

		const FString& Status = GameSession.Status;
		const FString& SessionId = GameSession.GameSessionId;

		HandleGameSessionStatus(Status, SessionId);
	}
}

void UGameSessionManager::CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		JoinStatusMessageDelegate.Broadcast(HTTPStatusMessage::SomethingWrong, true);
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			JoinStatusMessageDelegate.Broadcast(HTTPStatusMessage::SomethingWrong, true);
			return;
		}

		// Successful Response
		JoinStatusMessageDelegate.Broadcast(TEXT("게임에 접속합니다..."), false);
		UE_LOG(LogAuraDS, Log, TEXT("UGameSessionManager::TryCreatePlayerSession_Response, Successful!"));

		FDSPlayerSession PlayerSession;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &PlayerSession);

		PlayerSession.Dump();

		const FString DestinationIPAndPort =
			FString::Printf(TEXT("%s:%d"), *PlayerSession.IpAddress, PlayerSession.Port);

		const FName Address = *DestinationIPAndPort;
		FString LastMapName{};
		FString LastPlayerStartName{};
		bool bIsNewPlayer = false;
		bool bIsRespawn = false;

		if (UDS_LocalPlayerSubsystem* Subsystem = GetDSLocalPlayerSubsystem())
		{
			bIsNewPlayer = Subsystem->IsNewPlayer();
		}
		
		check(GameInstanceSubsystem);

		LastMapName = GameInstanceSubsystem->GetLastMapName();
		LastPlayerStartName = GameInstanceSubsystem->GetLastPlayerStartName();
		bIsRespawn = GameInstanceSubsystem->IsRespawn();

		const FString Option = FString::Printf(
			TEXT("?PlayerSessionId=%s?UserName=%s?Sub=%s?LastMapName=%s?LastPlayerStartName=%s?IsNewPlayer=%d?IsRespawn=%d"), 
			*PlayerSession.PlayerSessionId, 
			*PlayerSession.PlayerId, 
			*PlayerSession.Sub, 
			*LastMapName,
			*LastPlayerStartName,
			bIsNewPlayer,
			bIsRespawn
			);

		UGameplayStatics::OpenLevel(this, Address, true, Option); 
	}
}

void UGameSessionManager::HandleGameSessionStatus(const FString& Status, const FString& GameSessionId)
{
	if (Status.Equals(TEXT("ACTIVE")))
	{
		JoinStatusMessageDelegate.Broadcast(TEXT("접속중입니다. 잠시만 기다리세요..."), false);
		if (UDS_LocalPlayerSubsystem* SubSystem = GetDSLocalPlayerSubsystem(); IsValid(SubSystem)) {
			const FString UserName = SubSystem->GetLocalPlayerUserName();
			TryCreatePlayerSession(UserName, GameSessionId);
		}

	}
	else if (Status.Equals(TEXT("ACTIVATING")))
	{
		FTimerDelegate TryAgainDelegate;
		TryAgainDelegate.BindUObject(this, &ThisClass::JoinGameSession);
		APlayerController* PlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (IsValid(PlayerController))
		{
			PlayerController->GetWorldTimerManager().SetTimer(CreateSessionTimer, TryAgainDelegate, 0.5f, false);
		}
	}
	else
	{
		JoinStatusMessageDelegate.Broadcast(TEXT("게임 세션을 만들지 못했습니다..."), true);
		UE_LOG(LogAuraDS, Log, TEXT("Status: %s"), *Status);
	}
}

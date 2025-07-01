// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DS_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Character.h"
#include "Common/DedicatedServer.h"
#include "Player/DS_PlayerState.h"
#include "Player/DS_LocalPlayerSubsystem.h"

ADS_PlayerController::ADS_PlayerController()
{
	bReplicates = true;

	SingleTripTime = 0.f;
	PingInterval = 2.f;
}

void ADS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		PersistentLevelName = *FPackageName::GetShortName(GetWorld()->PersistentLevel->GetPackage()->GetLoadedPath().GetPackageName());
		UE_LOG(LogAuraDS, Log, TEXT("ADS_PlayerController::BeginPlay(), Persistent Level Name: %s"), *PersistentLevelName.ToString());
	}
}

void ADS_PlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void ADS_PlayerController::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();
}

void ADS_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADS_PlayerController, UserName);
}

void ADS_PlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (GetNetMode() == NM_Standalone)
		return;

	if (IsLocalController())
	{
		Server_Ping(GetWorld()->GetTimeSeconds());

		GetWorldTimerManager().SetTimer(PingTimerHandle, this, &ADS_PlayerController::SendPing, PingInterval, true);
	}
}

void ADS_PlayerController::Server_Ping_Implementation(float RequestTime)
{
	Client_Pong(RequestTime);
}

void ADS_PlayerController::Client_Pong_Implementation(float RequestTime)
{
	const float RoundTripTime = GetWorld()->GetTimeSeconds() - RequestTime;
	SingleTripTime = RoundTripTime * 0.5f;

	AverageSingleTripTime = FMath::Lerp(AverageSingleTripTime, SingleTripTime, 0.1f);

	// TODO: Use Average Trip Time To Warn!
}

void ADS_PlayerController::Client_TimerUpdate_Implementation(float RemainTime)
{
}

void ADS_PlayerController::Client_TimerStop_Implementation(float RemainTime)
{
}

void ADS_PlayerController::Client_SetInputEnable_Implementation(bool bEnable)
{
}

void ADS_PlayerController::Server_StartGame_Implementation()
{
}

void ADS_PlayerController::SendPing()
{
	Server_Ping(GetWorld()->GetTimeSeconds());
}

void ADS_PlayerController::SetUserInfos(const FString& SessionId, const FString& Name)
{
	PlayerSessionId = SessionId;
	UserName = Name;
}

void ADS_PlayerController::SetUserInfos(const FString& SessionId, const FString& Name, const FString& Sub)
{
	PlayerSessionId = SessionId;
	UserName = Name;
	if (ADS_PlayerState* DSPlayerState = Cast<ADS_PlayerState>(PlayerState))
	{
		DSPlayerState->SetUserInfo(Name, Sub);
	}
}

void ADS_PlayerController::SetSavedUserLocation(const FName& MapName, const FName& PlayerStart)
{
	CachedNextLevel = MapName;
	CachedNextPlayerStart = PlayerStart;
}

void ADS_PlayerController::LoadAndMoveToSavedLocation()
{
	LoadStreamingLevelAndMovePlayer(PersistentLevelName, CachedNextLevel, CachedNextPlayerStart);
}

void ADS_PlayerController::LoadStreamingLevelAndMovePlayer(const FName& CurrentLevelName, const FName& NextLevelName, const FName& PlayerStartName)
{
	UE_LOG(LogAuraDS, Log, TEXT("ADS_PlayerController::LoadStreamingLevelAndMovePlayer"));

	if (!HasAuthority())
		return;

	UE_LOG(LogAuraDS, Log, TEXT("LoadStreamingLevelAndMovePlayer(Server), TryLoadLevel: %s"), *NextLevelName.ToString());
	CachedCurrentLevel = CurrentLevelName;
	CachedNextLevel = NextLevelName;
	CachedNextPlayerStart = PlayerStartName;

	ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(this, NextLevelName);
	if (StreamingLevel)
	{
		if (!StreamingLevel->IsLevelLoaded())
			LoadLevel(StreamingLevel);
		else
			OnServerStreamingLevelLoaded();
	}
	else
	{
		UE_LOG(LogAuraDS, Log, TEXT("--- StreamingLevel Is NOT Valid, %s ---"), *FString(__FUNCTION__));
		TArray<ULevelStreaming*> Levels = GetWorld()->GetStreamingLevels();
		for (ULevelStreaming* Level : Levels)
		{
			if (Level)
			{
				UE_LOG(LogAuraDS, Log, TEXT("Streaming Level: %s"), *Level->GetWorldAssetPackageName());
			}
		}
	}
}

void ADS_PlayerController::LoadLevel(ULevelStreaming* Level)
{
	UE_LOG(LogAuraDS, Log, TEXT("Load Level()!"));

	check(Level);

	if (Level->IsLevelLoaded())
		return;

	if (HasAuthority())
	{
		Level->OnLevelShown.AddUniqueDynamic(this, &ADS_PlayerController::OnServerStreamingLevelLoaded);
	}
	else
	{
		Level->OnLevelShown.AddUniqueDynamic(this, &ADS_PlayerController::OnClientStreamingLevelLoaded);
	}

	Level->SetShouldBeLoaded(true);
	Level->SetShouldBeVisible(true);
}

void ADS_PlayerController::OnServerStreamingLevelLoaded()
{
	UE_LOG(LogAuraDS, Log, TEXT("OnServerStreamingLevelLoaded()"));

	MovePlayerLocation(CachedNextLevel, CachedNextPlayerStart);

	Client_LoadStreamingLevel(CachedCurrentLevel, CachedNextLevel, CachedNextPlayerStart);
	
	
}

void ADS_PlayerController::Client_LoadStreamingLevel_Implementation(const FName& CurrentLevelName, const FName& NextLevelName, const FName& PlayerStartName)
{
	UE_LOG(LogAuraDS, Log, TEXT("Client RPC, Load Level: %s"), *NextLevelName.ToString());
	
	CameraFadeOut();
	
	CachedCurrentLevel = CurrentLevelName;
	CachedNextLevel = NextLevelName;
	CachedNextPlayerStart = PlayerStartName;

	ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(this, NextLevelName);
	if (StreamingLevel)
	{
		if (!StreamingLevel->IsLevelLoaded())
		{
			UE_LOG(LogAuraDS, Log, TEXT("ADS_PlayerController::Client_LoadStreamingLevel, %s Is Not Loaded Yet"), *NextLevelName.ToString());
			LoadLevel(StreamingLevel);
		}
		else
		{
			UE_LOG(LogAuraDS, Log, TEXT("ADS_PlayerController::Client_LoadStreamingLevel, %s has Already Loaded"), *NextLevelName.ToString());
			OnClientStreamingLevelLoaded();
		}
	}
}


void ADS_PlayerController::OnClientStreamingLevelLoaded()
{
	UE_LOG(LogAuraDS, Log, TEXT("OnClientStreamingLevelLoaded()"));

	//UnloadPrevLevel();
	UnloadLevels();
	CameraFadeIn();

	CachedCurrentLevel = CachedNextLevel;
}

void ADS_PlayerController::MovePlayerLocation(const FName& NextLevel, const FName& PlayerStartName)
{
	UE_LOG(LogAuraDS, Log, TEXT("--- Server_MovePlayerLocation_Implementation() ---"));
	UE_LOG(LogAuraDS, Log, TEXT("NextLevelName: %s"), *NextLevel.ToString());
	UE_LOG(LogAuraDS, Log, TEXT("PlayerStartName: %s"), *PlayerStartName.ToString());

	// TODO : Remove This!
	for (ULevelStreaming* StreamingLevel : GetWorld()->GetStreamingLevels())
	{
		if (StreamingLevel)
		{
			UE_LOG(LogAuraDS, Log, TEXT("Level: %s | Loaded: %d | Visible: %d"),
				*StreamingLevel->GetWorldAssetPackageName(),
				StreamingLevel->IsLevelLoaded(),
				StreamingLevel->IsLevelVisible());
		}
	}

	ACharacter* OwnedCharacter = GetCharacter();

	if (OwnedCharacter)
	{
		for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
		{
			const FName& StartTag = It->PlayerStartTag;
			UE_LOG(LogAuraDS, Log, TEXT("StartTag In Array: %s"), *StartTag.ToString());

			if (StartTag == PlayerStartName)
			{
				OwnedCharacter->SetActorLocation(It->GetActorLocation());
				OwnedCharacter->SetActorRotation(It->GetActorRotation());

				return;
			}
		}
		UE_LOG(LogAuraDS, Warning, TEXT("CachedNextPlayerStart Is NOT Valid, %s"), *FString(__FUNCTION__));
	}
}


void ADS_PlayerController::UnloadPrevLevel()
{
	UE_LOG(LogAuraDS, Log, TEXT("--- ADS_PlayerController::UnloadPrevLevel ---"));

	if (CachedCurrentLevel.IsNone())
	{
		UE_LOG(LogAuraDS, Log, TEXT("CachedCurrentLevel.IsNone!"));
		return;
	}

	if (PersistentLevelName.IsNone())
	{
		UE_LOG(LogAuraDS, Log, TEXT("PersistentLevelName.IsNone!"));
		return;
	}

	if (CachedCurrentLevel == PersistentLevelName)
	{
		UE_LOG(LogAuraDS, Log, TEXT("CachedCurrentLevel == PersistentLevelName!"));
		return;
	}

	UE_LOG(LogAuraDS, Log, TEXT("Unload Level: %s"), *CachedCurrentLevel.ToString());

	UGameplayStatics::UnloadStreamLevel(this, CachedCurrentLevel, FLatentActionInfo(), false);
}

void ADS_PlayerController::UnloadLevels()
{
	UWorld* World = GetWorld();
	if (!World)
		return;
	
	UE_LOG(LogAuraDS, Log, TEXT("----- ADS_PlayerController::UnloadLevels"));
	UE_LOG(LogAuraDS, Log, TEXT("Next Level Name: %s"), *CachedNextLevel.ToString());
	UE_LOG(LogAuraDS, Log, TEXT("Persist Level Name: %s"), *PersistentLevelName.ToString());


	int32 UUIDDiff = 1;

	for (ULevelStreaming* StreamingLevel : World->GetStreamingLevels())
	{
		if (!StreamingLevel || !StreamingLevel->IsLevelLoaded())
			continue;

		ULevel* LoadedLevel = StreamingLevel->GetLoadedLevel();
		if (!LoadedLevel)
			continue;

		const FString& LoadedLevelPath = StreamingLevel->GetWorldAssetPackageName();
		FString LoadedLevelName = FPackageName::GetShortName(LoadedLevelPath);
		 
		// TODO: Remove This 
		if (LoadedLevelName.StartsWith(TEXT("UEDPIE_")))
		{
			int32 UnderscoreIndex;
			if (LoadedLevelName.FindChar('_', UnderscoreIndex))
			{
				LoadedLevelName = LoadedLevelName.RightChop(UnderscoreIndex + 3);
			}
		}

		UE_LOG(LogAuraDS, Log, TEXT("LoadedLevelName: %s"), *LoadedLevelName);

		if (LoadedLevel->IsPersistentLevel() || LoadedLevelName == CachedNextLevel.ToString())
		{
			UE_LOG(LogAuraDS, Log, TEXT("PersistentLevel || NextLevel"));
			continue;
		}

		UE_LOG(LogAuraDS, Log, TEXT("*** UnLoad %s Level"), *LoadedLevelName);

		//StreamingLevel->SetShouldBeVisible(false);
		//StreamingLevel->SetShouldBeLoaded(false);
		
		// TEMP
		FString LevelPath = TEXT("/Game/Maps/") + LoadedLevelName;
		const FName UnloadLevelPath(*LevelPath);
		
		UE_LOG(LogAuraDS, Log, TEXT("UnLoad Level Path: %s"), *UnloadLevelPath.ToString());

		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = FName(TEXT("OnLevelUnloadCallback"));
		LatentInfo.Linkage = 0;
		LatentInfo.UUID = __LINE__ + UUIDDiff++;
		
		UGameplayStatics::UnloadStreamLevel(this, UnloadLevelPath, LatentInfo, false);
	}
}

void ADS_PlayerController::OnLevelUnloadCallback()
{
	UE_LOG(LogAuraDS, Log, TEXT("~~~~~ OnLevelUnloadCallback ~~~~~"));

	UWorld* World = GetWorld();
	if (!World)
		return;
	
	int32 Count = 0;
	for (ULevelStreaming* StreamingLevel : World->GetStreamingLevels())
	{
		if (StreamingLevel && StreamingLevel->IsLevelLoaded())
		{
			++Count;
			UE_LOG(LogAuraDS, Log, TEXT("   %d. %s"), Count, *StreamingLevel->GetWorldAssetPackageName());
		}
	}
}


void ADS_PlayerController::CameraFadeOut()
{
	UE_LOG(LogAuraDS, Log, TEXT("ADS_PlayerController::CameraFadeOut()"));
	ClientSetCameraFade(true, FColor::Black, FVector2D(0.f, 1.f), 0.f, false, true);
}

void ADS_PlayerController::CameraFadeIn()
{
	UE_LOG(LogAuraDS, Log, TEXT("ADS_PlayerController::CameraFadeIn()"));
	ClientSetCameraFade(true, FColor::Black, FVector2D(1.f, 0.f), 0.5f, false, false);
}






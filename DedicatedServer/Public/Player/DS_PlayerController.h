// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DS_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API ADS_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADS_PlayerController();

	virtual void ReceivedPlayer() override;

	UFUNCTION(Server, Reliable)
	void Server_Ping(float RequestTime);

	UFUNCTION(Client, Reliable)
	void Client_Pong(float RequestTime);

	UFUNCTION(Client, Reliable)
	void Client_TimerUpdate(float RemainTime);

	UFUNCTION(Client, Reliable)
	void Client_TimerStop(float RemainTime);

	const FString& GetPlayerSessionId() const { return PlayerSessionId; }

	UFUNCTION(BlueprintCallable)
	const FString& GetPlayerName() const { return UserName; }

	void SetUserInfos(const FString& SessionId, const FString& Name);
	void SetUserInfos(const FString& SessionId, const FString& Name, const FString& Sub);
	void SetSavedUserLocation(const FName& MapName, const FName& PlayerStart);
	void LoadAndMoveToSavedLocation();

	UFUNCTION(Client, Reliable)
	void Client_SetInputEnable(bool bEnable);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_StartGame();

	UFUNCTION(Client, Reliable)
	void Client_LoadStreamingLevel(const FName& CurrentLevelName, const FName& NextLevelName, const FName& PlayerStartName);

	void LoadStreamingLevelAndMovePlayer(const FName& CurrentLevelName, const FName& NextLevelName, const FName& PlayerStartName);

	void MovePlayerLocation(const FName& NextLevel, const FName& PlayerStartName);

	bool IsNewPlayer() { return bNewPlayer; }
	bool IsRespawn() { return bIsRespawn; }
	void SetIsNewPlayer(bool IsNewPlayer) { bNewPlayer = IsNewPlayer; }
	void SetIsRespawn(bool IsRespawn) { bIsRespawn = IsRespawn; }
	FString GetCurrentLevelName() { return CachedCurrentLevel.ToString(); }

	virtual void SendMessageToAllClient(const FString& SenderName, const FString& Chat) {};

protected:
	virtual void BeginPlay() override;
	virtual void OnRep_PlayerState() override;
	virtual void PostSeamlessTravel() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnClientStreamingLevelLoaded();

	FString PlayerSessionId{};

	UPROPERTY(Replicated)
	FString UserName{};

private:

	UFUNCTION()
	void OnServerStreamingLevelLoaded();

	void CameraFadeOut();

	void CameraFadeIn();

	void SendPing();

	void LoadLevel(ULevelStreaming* Level);
	void UnloadPrevLevel();
	void UnloadLevels();
	
	UFUNCTION()
	void OnLevelUnloadCallback();

	UPROPERTY()
	FTimerHandle PingTimerHandle;

	float SingleTripTime;
	float AverageSingleTripTime;
	float PingInterval;

	FName PersistentLevelName;
	FName CachedCurrentLevel;
	FName CachedNextLevel;
	FName CachedNextPlayerStart;

	bool bNewPlayer = false;
	bool bIsRespawn = false;
};

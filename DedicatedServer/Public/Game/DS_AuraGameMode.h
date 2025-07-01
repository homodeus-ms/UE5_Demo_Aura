// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/DS_GameModeBase.h"
#include "GameLiftServerSDK.h"
#include "DS_AuraGameMode.generated.h"

class UDS_GameInstanceSubsystem;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API ADS_AuraGameMode : public ADS_GameModeBase
{
	GENERATED_BODY()
	
public:
	ADS_AuraGameMode();

protected:
	virtual void BeginPlay() override;

	virtual void PreLogin(const FString& Options,
		const FString& Address,
		const FUniqueNetIdRepl& UniqueId,
		FString& ErrorMessage) override;

	virtual FString InitNewPlayer(APlayerController* NewPlayerController,
		const FUniqueNetIdRepl& UniqueId,
		const FString& Options,
		const FString& Portal = TEXT("")) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Existing) override;

	UPROPERTY(EditDefaultsOnly, Category = "Level")
	FName DefaultLevelName{};

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	void RemovePlayerSession(AController* Existing);

private:
	bool TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& PlayerId, FString& OUTErrorMessage);
	void ReturnToPortal();
	FName GetPlayerStartName();

	void InitGameLift();
	void SetServerParameteres(FServerParameters& OUTServerParams);

	UPROPERTY()
	TObjectPtr<UDS_GameInstanceSubsystem> DSGameInstanceSubsystem;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DestinationMap;

};

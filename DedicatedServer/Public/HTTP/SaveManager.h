// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HTTP/HTTPRequestManager.h"
#include "Interfaces/SaveManageInterface.h"
#include "Save/AuraSaveData.h"
#include "SaveManager.generated.h"

class UAuraSaveData;
class ADS_PlayerState;
class UDS_GameInstanceSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitInfoLoaded, const FPlayerInfoSaveData&, PlayerInfoData);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DEDICATEDSERVER_API USaveManager : public UHTTPRequestManager, public ISaveManageInterface
{
	GENERATED_BODY()
	
public:
	// ISaveManageInterface
	virtual void SaveGame(ADS_PlayerState* DSPlayerState) override;
	virtual void LoadGame(const FString& Sub, FOnRetrieveInfosDelegate Callback) override;
	virtual FOnRetrieveInfosDelegate& GetRetrieveInfosDelegate() override;
	virtual void SetNotANewPlayer(ADS_PlayerState* DSPlayerState) override;

	void SetGameInstanceSubsystem(UDS_GameInstanceSubsystem* InSubsystem) { GameInstanceSubsystem = InSubsystem; }
	void LoadInitInfo(const FString& Sub);

	USaveManager();

	FOnInitInfoLoaded OnInitInfoLoaded;
	FStatusMessageDelegate SaveStatusMessageDelegate;
	FOnRetrieveInfosDelegate OnRetrieveInfos;

protected:
	

private:
	void SaveGame_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void LoadGame_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void LoadInitInfo_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void SetNotANewPlayer_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void SetSaveDataParams(FAPIRequestSendData& OUTSendData);
	UAuraSaveData* ConvertRetrieveDataToStruct(TSharedPtr<FJsonObject> JsonObject);
	void KeepPlayersLastPos(const FString& LastMapName, const FString& LastPlayerStartName);
	
	TMap<FString, FOnRetrieveInfosDelegate> RetrieveCallbacksMap;

	UPROPERTY()
	TObjectPtr<UDS_GameInstanceSubsystem> GameInstanceSubsystem;
};

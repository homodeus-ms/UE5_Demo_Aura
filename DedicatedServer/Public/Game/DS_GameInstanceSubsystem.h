// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameLiftServerSDK.h"
#include "DS_GameInstanceSubsystem.generated.h"

class USaveManager;
class UAuraSaveData;
class ISaveManageInterface;
class UGameSessionManager;
class IGameSessionManageInterface;

/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API UDS_GameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UDS_GameInstanceSubsystem();
	void InitGameLift(const FServerParameters& ServerParameters);

	UPROPERTY(BlueprintReadOnly)
	bool bGameLiftInitialized = false;

	USaveManager* GetSaveManager() const;
	ISaveManageInterface* GetSaveManageInterface();
	UGameSessionManager* GetGameSessionManager() const;
	IGameSessionManageInterface* GetGameSessionManageInterface();

	const FString& GetLastMapName() const { return LastMapName; }
	const FString& GetLastPlayerStartName() const { return LastPlayerStartName; }
	bool IsRespawn() const { return bIsRespawn; }
	
	void KeepLastMapName(const FString& MapName);
	void KeepLastPlayerStartName(const FString& PlayerStartName);
	void SetIsRespawn(bool IsRespawn) { bIsRespawn = IsRespawn; }

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection);

private:
	void SetProcessParameters(FGameLiftServerSDKModule* GameLiftSDKModule);
	int32 GetPortNumber();
	FProcessParameters ProcessParams;

	UPROPERTY()
	TSubclassOf<USaveManager> SaveManagerClass;

	UPROPERTY()
	TObjectPtr<USaveManager> SaveManager;

	UPROPERTY()
	TSubclassOf<UGameSessionManager> GameSessionManagerClass;

	UPROPERTY()
	TObjectPtr<UGameSessionManager> GameSessionManager;

	FString LastMapName{};
	FString LastPlayerStartName{};

	bool bIsRespawn = false;
};

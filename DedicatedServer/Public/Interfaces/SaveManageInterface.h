// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveManageInterface.generated.h"

class ADS_PlayerState;
class UAuraSaveData;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRetrieveInfosDelegate, class UAuraSaveData*, SaveData);
DECLARE_DELEGATE_OneParam(FOnRetrieveInfosDelegate, UAuraSaveData* /* SaveData */);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveManageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEDICATEDSERVER_API ISaveManageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SaveGame(ADS_PlayerState* DSPlayerState) abstract;
	virtual void LoadGame(const FString& Sub, FOnRetrieveInfosDelegate Callback) abstract;
	virtual FOnRetrieveInfosDelegate& GetRetrieveInfosDelegate() abstract;
	virtual void SetNotANewPlayer(ADS_PlayerState* DSPlayerState) abstract;
};

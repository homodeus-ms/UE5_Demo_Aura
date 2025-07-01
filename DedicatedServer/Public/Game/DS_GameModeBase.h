// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DS_GameModeBase.generated.h"

class UDS_GameInstanceSubsystem;
class ISaveManageInterface;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API ADS_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UDS_GameInstanceSubsystem* GetDSGameInstanceSubsystem() const;
	ISaveManageInterface* GetSaveManageInterface();
};

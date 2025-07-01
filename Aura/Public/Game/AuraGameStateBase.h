// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Interface/StageClearObserver.h"
#include "Data/StageClearCondition.h"
#include "AuraGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraGameStateBase : public AGameStateBase, public IStageClearObserver
{
	GENERATED_BODY()
	
public:
	// Stage Clear Observer 
	virtual void SetMapInfo() override;
	virtual void OnTriggerActivated() override;
	virtual void OnEnemyKilled() override;
	
	bool CheckStageCleared();

protected:

	virtual void BeginPlay() override;

	UPROPERTY()
	TMap<FString, FStageClearInfo> StageClearInfo;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStageClearCondition> StageClearData;

	FString CurrentMapName = FString();

	FStageClearInfo CurrentInfo;

private:
	
	void ActivateNextMapEntrance();

	FTimerHandle StageClearedTimer;
	float StageClearedEventDelay = 2.f;
};

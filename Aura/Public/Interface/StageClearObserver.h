// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StageClearObserver.generated.h"

UINTERFACE(MinimalAPI)
class UStageClearObserver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IStageClearObserver
{
	GENERATED_BODY()

public:
	virtual void SetMapInfo() abstract;
	virtual void OnTriggerActivated() abstract;
	virtual void OnEnemyKilled() abstract;

};

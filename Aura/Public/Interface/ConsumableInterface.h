// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ConsumableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UConsumableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IConsumableInterface
{
	GENERATED_BODY()

public:

	virtual void Spend(AActor* Target) = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OpenableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOpenableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IOpenableInterface
{
	GENERATED_BODY()

public:
	virtual void OnSelected() abstract;
	virtual void OnDeselected() abstract;
	virtual void OpenBox(AActor* Actor) abstract;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowBoxOpenEffect();
	
};

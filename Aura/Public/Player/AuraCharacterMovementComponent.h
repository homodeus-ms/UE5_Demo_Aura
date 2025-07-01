// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AuraCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:

	virtual void ServerMove_PerformMovement(const FCharacterNetworkMoveData& MoveData) override;
	
};

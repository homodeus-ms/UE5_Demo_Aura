// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraCharacterMovementComponent.h"
#include "GameFramework/Character.h"

void UAuraCharacterMovementComponent::ServerMove_PerformMovement(const FCharacterNetworkMoveData& MoveData)
{
	Super::ServerMove_PerformMovement(MoveData);

	if (CharacterOwner && !CharacterOwner->bClientUpdating && CharacterOwner->IsPlayingRootMotion() && CharacterOwner->GetMesh())
	{
		CharacterOwner->GetMesh()->ConditionallyDispatchQueuedAnimEvents();
	}
}

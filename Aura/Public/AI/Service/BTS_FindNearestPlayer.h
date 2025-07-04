// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_FindNearestPlayer.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UBTS_FindNearestPlayer : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_FindNearestPlayer();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


};

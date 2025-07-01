// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SpawnMinions.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UBTTask_SpawnMinions : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SpawnMinions();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

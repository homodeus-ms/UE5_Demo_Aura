// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ShouldSpawnMinions.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UBTDecorator_ShouldSpawnMinions : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_ShouldSpawnMinions();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};

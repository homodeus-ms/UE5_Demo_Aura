// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_ShouldSpawnMinions.h"

UBTDecorator_ShouldSpawnMinions::UBTDecorator_ShouldSpawnMinions()
{
}

bool UBTDecorator_ShouldSpawnMinions::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return true;
}

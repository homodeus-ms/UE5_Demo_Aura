// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AuraAIController.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AAuraAIController();

	UFUNCTION(BlueprintCallable)
	void SetTimerByBoolVarName(FName VarName, bool bValue, float Time);
	

protected:
	TObjectPtr<class UBehaviorTreeComponent> BehaviorTreeComponent;

private:
	UFUNCTION()
	void OnBoolTimerFinished(FName VarName, bool bValue);

	FTimerHandle Timer;
};

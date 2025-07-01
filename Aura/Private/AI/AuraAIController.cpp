// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AuraAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AuraLog.h"

AAuraAIController::AAuraAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void AAuraAIController::SetTimerByBoolVarName(FName VarName, bool bValue, float Time)
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ThisClass::OnBoolTimerFinished, VarName, bValue);

	GetWorld()->GetTimerManager().SetTimer(Timer, TimerDelegate, Time, false);
}

void AAuraAIController::OnBoolTimerFinished(FName VarName, bool bValue)
{
	Timer.Invalidate();
	if (UBlackboardComponent* BB = BehaviorTreeComponent->GetBlackboardComponent())
	{
		const FBlackboard::FKey KeyID = BB->GetKeyID(VarName);
		
		if (!BB->IsValidKey(KeyID))
		{
			UE_LOG(Aura, Warning, TEXT("Blackboard key '%s' does not exist!"), *VarName.ToString());
			return;
		}

		BB->SetValueAsBool(VarName, bValue);
	}
}

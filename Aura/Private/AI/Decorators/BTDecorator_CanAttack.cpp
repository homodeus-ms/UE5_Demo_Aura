// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_CanAttack.h"
#include "Interface/EnemyInterface.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AuraAIDefines.h"
#include "AuraLog.h"

UBTDecorator_CanAttack::UBTDecorator_CanAttack()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (!Super::CalculateRawConditionValue(OwnerComp, NodeMemory))
		return false;

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn)
		return false;

	IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(ControlledPawn);
	if (!EnemyInterface)
		return false;

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (!Target)
	{
		CheckValueOnScreen("TargetIsNull", 0);
		return false;
	}

	const float AttackRange = EnemyInterface->GetAttackRange();
	const float DistanceToTarget = ControlledPawn->GetDistanceTo(Target);

	bool bResult = (AttackRange >= DistanceToTarget);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_bTARGET_IN_ATTACK_RANGE, bResult);

	return bResult;

}

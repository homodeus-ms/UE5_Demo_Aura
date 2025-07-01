// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_SpawnMinions.h"
#include "AI/AuraAIController.h"
#include "Character/AuraEnemy.h"
#include "AbilitySystemComponent.h"

UBTTask_SpawnMinions::UBTTask_SpawnMinions()
{
}

EBTNodeResult::Type UBTTask_SpawnMinions::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAuraAIController* AIController = Cast<AAuraAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
        return EBTNodeResult::Failed;

    AAuraEnemy* Shaman = Cast<AAuraEnemy>(AIController->GetPawn());
    if (!IsValid(Shaman) && Shaman->GetCharacterType() != ECharacterType::Magician)
        return EBTNodeResult::Failed;

    Shaman->GetAbilitySystemComponent();

    return EBTNodeResult::Succeeded;
}

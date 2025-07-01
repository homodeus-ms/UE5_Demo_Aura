// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTS_FindNearestPlayer.h"
#include "AI/AuraAIController.h"
#include "Interface/EnemyInterface.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AuraAIDefines.h"
#include "AuraLog.h"
#include "Interface/PlayerInterface.h"


UBTS_FindNearestPlayer::UBTS_FindNearestPlayer()
{
	NodeName = TEXT("Detect Player");
	Interval = 0.5f;
}

void UBTS_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
		return;

	AAuraAIController* AuraAIController = Cast<AAuraAIController>(AIController);
	if (!AuraAIController)
		return;

	UWorld* World = ControlledPawn->GetWorld();
	if (!World)
		return;

	IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(ControlledPawn);
	if (!EnemyInterface)
		return;

	FVector EnemyLocation = ControlledPawn->GetActorLocation();
	const float DetectRange = EnemyInterface->GetDetactRange();

	TArray<FOverlapResult> DetectedPawns;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(Detect), false, ControlledPawn);
	bool bDetected = World->OverlapMultiByChannel(
		DetectedPawns,
		EnemyLocation,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(DetectRange),
		QueryParams
	);

	float MinDist = MAX_FLT;
	AActor* TargetActor = nullptr;
	//OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);

	if (bDetected)
	{
		for (const FOverlapResult& Overlapped : DetectedPawns)
		{
			AActor* OverlappedActor = Overlapped.GetActor();

			if (!OverlappedActor->ActorHasTag(FName(TEXT("Player"))))
			{
				continue;
			}

			// On Spawn Protection
			if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OverlappedActor))
			{
				if (PlayerInterface->IsInvincible())
				{
					continue;
				}
			}

			FVector TargetLocation = OverlappedActor->GetActorLocation();
			float Distance = ControlledPawn->GetDistanceTo(OverlappedActor);
			if (MinDist > Distance)
			{
				MinDist = Distance;
				TargetActor = OverlappedActor;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, TargetActor);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_DIST_TO_TARGET, MinDist);

}

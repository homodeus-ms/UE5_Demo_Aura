// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume/EnemySpawnVolume.h"
#include "Components/BoxComponent.h"
#include "SpawnVolume/EnemySpawnPoint.h"
#include "Interface/PlayerInterface.h"
#include "Interface/StageClearObserver.h"
#include "AuraGameUtility.h"

AEnemySpawnVolume::AEnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	SetRootComponent(BoxTrigger);
	BoxTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxTrigger->SetCollisionObjectType(ECC_WorldStatic);
	BoxTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
}

void AEnemySpawnVolume::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
		return;

	if (!OtherActor->Implements<UPlayerInterface>())
		return;

	if (TScriptInterface<IStageClearObserver> StageClearObserver = UAuraGameUtility::GetStageClearObserver(this))
	{
		StageClearObserver->OnTriggerActivated();
	}

	for (AEnemySpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (IsValid(SpawnPoint))
			SpawnPoint->SpawnEnemy();
	}

	Destroy();
}



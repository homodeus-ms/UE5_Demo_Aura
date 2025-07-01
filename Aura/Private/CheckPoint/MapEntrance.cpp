// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint/MapEntrance.h"
#include "Interface/PlayerInterface.h"
#include "AuraLog.h"
#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "SaveGame/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Player/AuraPlayerController.h"


AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMapEntrance::BeginPlay()
{
	Super::BeginPlay();
}

void AMapEntrance::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!NextMap.ToSoftObjectPath().IsValid())
	{
		UE_LOG(Aura, Warning, TEXT("Next Map Is NOT Valid, %s"), *FString(__FUNCTION__));
		return;
	}

	if (HasAuthority())
	{
		const FName CurrentLevelName = GetLevel()->GetOuter()->GetFName();
		const FName LevelName = FName(*NextMap.GetAssetName());

		if (ACharacter* Character = Cast<ACharacter>(OtherActor))
		{
			if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(Character))
			{
				PlayerInterface->SaveCurrentGame(LevelName.ToString(), NextMapPlayerStartName);
			}

			if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Character->GetController()))
			{
				PC->LoadStreamingLevelAndMovePlayer(CurrentLevelName, LevelName, NextMapPlayerStartName);
			}
		}
	}
	

	//if (!bStageCleared || !bHasNextMap)
	//	return;

	//if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OtherActor))
	//{
	//	// TEMP
	//	AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	//	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(AuraGM->GetGameInstance());
	//	ULoadScreenSaveGame* SaveGame = AuraGM->GetSavedData(AuraGI->LoadSlotName, AuraGI->SlotIndex);

	//	if (NextMap.ToSoftObjectPath().IsValid())
	//	{
	//		AuraGM->SaveMapNamesToSaveGameInstance(NextMap);

	//		PlayerInterface->SaveCurrentGame(NextMapPlayerStartName);

	//		UGameplayStatics::OpenLevel(this, AuraGM->GetMapAssetName(NextMap));
	//	}
	//	else
	//	{
	//		CheckValueOnScreen("NextMap Is Not Valid", 1);
	//	}
	//}
	
}

void AMapEntrance::OnStageCleared_Implementation()
{
	CheckValueOnScreen("OnCleared!!!", 1);
	bStageCleared = true;
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DedicatedServer/Public/Game/DS_AuraGameMode.h"
#include "AuraGameModeBase.generated.h"

class UMVVM_LoadSlots;
class UCharacterClassInfo;
class UAuraSaveData;
class UActorFactoryMap;
/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public ADS_AuraGameMode
{
	GENERATED_BODY()
	
public:

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UPROPERTY(EditDefaultsOnly, Category="Character Class")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "AbilityInfo")
	TObjectPtr<class UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly, Category = "TreasureBoxItemsInfo")
	TObjectPtr<class UTreasureBoxItemsInfo> TreasureBoxItemsInfo;

	class UAuraGameInstance* GetAuraGameInstance();

	//FString GetMapNameByMapAsset(TSoftObjectPtr<UWorld> MapAsset);
	//FName GetMapAssetName(TSoftObjectPtr<UWorld> MapAsset);
	//

	//// Save, Load

	//void SaveMapNamesToSaveGameInstance(TSoftObjectPtr<UWorld> MapAsset);
	//
	//UFUNCTION()
	//ULoadScreenSaveGame* FindSaveData();

	//UFUNCTION()
	//void SaveSlotDataByEnterNameButton(UMVVM_LoadSlots* LoadSlotVM, int32 SlotIndex);

	//UFUNCTION()
	//void DeleteSavedData(const FString& SlotName, int32 SlotIndex);

	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ULoadScreenSaveGame> LoadScreenSaveGameClass;

	/* 
	* Map 
	*/


	UPROPERTY(EditDefaultsOnly, Category="Map")
	FString DefaultMapName = FString("DefaultMap");

	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TSoftObjectPtr<UWorld> DefaultMap;


	/* 
	* Starting point
	*/

	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;

	/*
	* Spawn Event Actors
	*/
	UActorFactoryMap* GetActorFactoryMap() const { return FactoryMap; }

	void PlayerDied(AActor* DeadCharacter);

protected:

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "ActorFactoryMap")
	TSubclassOf<class UActorFactoryMap> ActorFactoryMapClass;

	UPROPERTY()
	TObjectPtr<class UActorFactoryMap> FactoryMap;

};

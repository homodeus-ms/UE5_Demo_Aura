// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DS_PlayerState.generated.h"

class USaveManager;
class UAuraSaveData;
class ISaveManageInterface;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API ADS_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	UAuraSaveData* GetSaveData();
	void ResetSaveData();

	ISaveManageInterface* GetSaveManageInterface();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*const FString& GetLastMapName() const;
	
	void SetLastMapName(const FString& MapName);*/

	UFUNCTION(BlueprintCallable)
	const FString& GetLocalPlayerUserName() const;

	FORCEINLINE const FString& GetSub() const { return Sub; }

	void SetUserInfo(const FString& InUserName, const FString& InSub);
	void SavePlayersLastMapInfoToGISubsystem(const FString& MapName, const FString& PlayerStartName);


protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TScriptInterface<ISaveManageInterface> SaveManageInterface;

	UPROPERTY()
	TObjectPtr<UAuraSaveData> SaveData = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName = TEXT("StartMap");

	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartName = FName("StartMapDefault");

	UPROPERTY(ReplicatedUsing = OnRep_UserName)
	FString UserName{};

	UPROPERTY()
	FString Sub{};

	UPROPERTY(ReplicatedUsing = OnRep_LastMapName)
	FString LastMapName;

	UPROPERTY(ReplicatedUsing = OnRep_LastPlayerStartName)
	FString LastPlayerStartName;

	UFUNCTION()
	void OnRep_LastMapName();

	UFUNCTION()
	void OnRep_LastPlayerStartName();

	UFUNCTION()
	void OnRep_UserName();
};

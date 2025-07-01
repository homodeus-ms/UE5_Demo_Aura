// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DashboardOverlay.generated.h"

class UGameSessionManager;
class USaveManager;
class UGamePage;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API UDashboardOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetManager(UGameSessionManager* InGameSessionManager);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveManager> SaveManagerClass;

	UPROPERTY()
	TObjectPtr<USaveManager> SaveManager;

	UPROPERTY()
	TObjectPtr<UGameSessionManager> GameSessionManager;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGamePage> GamePage;
};

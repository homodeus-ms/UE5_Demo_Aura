// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Portal/PageWidgetBase.h"
#include "Save/AuraSaveData.h"
#include "GamePage.generated.h"

class UTextBlock;
class UDS_Button;
class UDS_StatusMessageWidget;
class UGameSessionManager;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API UGamePage : public UPageWidgetBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetStatusMesage(const FString& Message, bool bResetButton);
	void SetManager(UGameSessionManager* InGameSessionManager);

	UFUNCTION()
	void SetInitInfo(const FPlayerInfoSaveData& PlayerInfoData);

protected:
	virtual void NativeConstruct() override;

private:

	UFUNCTION()
	void OnStartButtonClicked();

	UPROPERTY()
	TObjectPtr<UGameSessionManager> GameSessionManager;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_UserName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Level;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_MapName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDS_Button> Button_Join;
};

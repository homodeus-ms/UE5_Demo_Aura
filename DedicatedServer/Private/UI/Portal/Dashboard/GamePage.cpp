// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dashboard/GamePage.h"
#include "UI/Common/DS_StatusMessageWidget.h"
#include "UI/Common/DS_Button.h"
#include "Components/TextBlock.h"
#include "Game/DS_GameModeBase.h"
#include "Player/DS_LocalPlayerSubsystem.h"
#include "HTTP/GameSessionManager.h"
#include "Common/DedicatedServer.h"

void UGamePage::SetStatusMesage(const FString& Message, bool bResetButton)
{
	Super::UpdateStatusMessage(Message);
	Button_Join->SetButtonEnable(bResetButton);
}

void UGamePage::SetManager(UGameSessionManager* InGameSessionManager)
{
	GameSessionManager = InGameSessionManager;

	GameSessionManager->JoinStatusMessageDelegate.AddDynamic(this, &ThisClass::SetStatusMesage);
}

void UGamePage::SetInitInfo(const FPlayerInfoSaveData& PlayerInfoData)
{
	TextBlock_Level->SetText(FText::AsNumber(PlayerInfoData.PlayerLevel));
	TextBlock_MapName->SetText(FText::FromString(PlayerInfoData.MapName));
}

void UGamePage::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		UDS_LocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UDS_LocalPlayerSubsystem>();
		if (Subsystem)
		{
			TextBlock_UserName->SetText(FText::FromString(Subsystem->GetLocalPlayerUserName()));
		}
	}

	Button_Join->Button->OnClicked.AddDynamic(this, &ThisClass::OnStartButtonClicked);
}

void UGamePage::OnStartButtonClicked()
{
	UE_LOG(LogAuraDS, Log, TEXT("OnStartButtonClicked Func!"));
	Button_Join->SetIsEnabled(false);
	check(GameSessionManager);
	GameSessionManager->JoinGameSession();
}

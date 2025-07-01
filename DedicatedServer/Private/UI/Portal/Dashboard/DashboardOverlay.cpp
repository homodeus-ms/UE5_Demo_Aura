// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dashboard/DashboardOverlay.h"
#include "HTTP/GameSessionManager.h"
#include "HTTP/SaveManager.h"
#include "UI/Portal/Dashboard/GamePage.h"
#include "Player/DS_LocalPlayerSubsystem.h"
#include "Game/DS_GameInstanceSubsystem.h"


void UDashboardOverlay::SetManager(UGameSessionManager* InGameSessionManager)
{
	GameSessionManager = InGameSessionManager;
	check(GamePage);
	GamePage->SetManager(InGameSessionManager);
}

void UDashboardOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	check(GamePage);

	
	UDS_GameInstanceSubsystem* GISubsystem = GetGameInstance()->GetSubsystem<UDS_GameInstanceSubsystem>();
	SaveManager = GISubsystem->GetSaveManager();
	check(SaveManager);
	SaveManager->OnInitInfoLoaded.AddDynamic(GamePage, &UGamePage::SetInitInfo);

	FString Sub{};
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		UDS_LocalPlayerSubsystem* LPSubsystem = LocalPlayer->GetSubsystem<UDS_LocalPlayerSubsystem>();
		if (LPSubsystem)
		{
			Sub = LPSubsystem->GetSub();
		}
	}

	check(!Sub.IsEmpty());

	SaveManager->LoadInitInfo(Sub);
}

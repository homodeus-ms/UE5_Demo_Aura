// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/PortalHUD.h"
#include "UI/Portal/SignIn/SignInOverlay.h"
#include "UI/Portal/Dashboard/DashboardOverlay.h"
#include "HTTP/PortalManager.h"
#include "HTTP/GameSessionManager.h"
#include "Game/DS_GameInstanceSubsystem.h"


void APortalHUD::BeginPlay()
{
	Super::BeginPlay();

	check(IsValid(SignInOverlayClass));
	check(IsValid(PortalManagerClass));

	InitManager();

	APlayerController* PlayerController = GetOwningPlayerController();
	SignInOverlay = CreateWidget<USignInOverlay>(PlayerController, SignInOverlayClass);
	if (IsValid(SignInOverlay))
	{
		SignInOverlay->SetManager(PortalManager);
		SignInOverlay->AddToViewport();
	}

	FInputModeGameAndUI InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(true);
}

void APortalHUD::OnSignIn()
{
	if (IsValid(SignInOverlay))
	{
		SignInOverlay->RemoveFromParent();
	}

	APlayerController* PlayerController = GetOwningPlayerController();
	DashboardOverlay = CreateWidget<UDashboardOverlay>(PlayerController, DashboardOverlayClass);
	
	if (IsValid(DashboardOverlay))
	{ 
		check(IsValid(GameSessionManager));

		DashboardOverlay->SetManager(GameSessionManager);
		DashboardOverlay->AddToViewport();
	}
}

void APortalHUD::OnSignOut()
{
}

void APortalHUD::InitManager()
{
	if (!IsValid(PortalManager))
	{
		PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);
	}

	if (!IsValid(GameSessionManager))
	{
		if (UDS_GameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UDS_GameInstanceSubsystem>())
		{
			GameSessionManager = Subsystem->GetGameSessionManager();
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interfaces/HUDManageInterface.h"
#include "PortalHUD.generated.h"

class USignInOverlay;
class UDashboardOverlay;
class UPortalManager;
class UGameSessionManager;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API APortalHUD : public AHUD, public IHUDManageInterface
{
	GENERATED_BODY()
	
public:
	// HUD Manage Interface
	virtual void OnSignIn() override;
	virtual void OnSignOut() override;

	void InitManager();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USignInOverlay> SignInOverlayClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDashboardOverlay> DashboardOverlayClass;

	UPROPERTY()
	TObjectPtr<USignInOverlay> SignInOverlay;

	UPROPERTY()
	TObjectPtr<UDashboardOverlay> DashboardOverlay;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPortalManager> PortalManagerClass;

	UPROPERTY()
	TObjectPtr<UPortalManager> PortalManager;

	UPROPERTY()
	TObjectPtr<UGameSessionManager> GameSessionManager;
	
};

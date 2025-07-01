// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DS_LocalPlayerSubsystem.h"
#include "HTTP/PortalManager.h"

void UDS_LocalPlayerSubsystem::InitiateToken(const FDSAuthenticationResult& InAuthResult, UPortalManager* InPortalManager)
{
	AuthResult = InAuthResult;
	PortalManagementInterface = InPortalManager;

	SetRefreshTokenTimer();
}

void UDS_LocalPlayerSubsystem::UpdateToken(const FDSAuthenticationResult& NewAuthResult)
{
	AuthResult.AccessToken = NewAuthResult.AccessToken;
	AuthResult.IdToken = NewAuthResult.IdToken;
	AuthResult.Dump();
	SetRefreshTokenTimer();
}

void UDS_LocalPlayerSubsystem::SetRefreshTokenTimer()
{
	UWorld* World = GetWorld();
	if (IsValid(World) && IsValid(PortalManagementInterface.GetObject()))
	{
		FTimerDelegate Delegate;
		Delegate.BindLambda([this]()
			{
				PortalManagementInterface->RefreshToken(AuthResult.RefreshToken);
			});
		World->GetTimerManager().SetTimer(RefreshTimer, Delegate, RefreshTokenInterval, false);
	}
}

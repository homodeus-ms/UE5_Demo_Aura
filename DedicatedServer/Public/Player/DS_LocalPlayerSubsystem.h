// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "DedicatedServer/HTTPRequestTypes.h"
#include "Interfaces/PortalManageInterface.h"
#include "DS_LocalPlayerSubsystem.generated.h"

class UPortalManager;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API UDS_LocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	void InitiateToken(const FDSAuthenticationResult& InAuthResult, UPortalManager* InPortalManager);
	void UpdateToken(const FDSAuthenticationResult& NewAuthResult);
	TScriptInterface<IPortalManageInterface> GetPortalManagementInterface() const { return PortalManagementInterface; }
	const FDSAuthenticationResult& GetAuthResult() const { return AuthResult; }

	UFUNCTION(BlueprintCallable)
	const FString& GetLocalPlayerUserName() const { return UserName; }
	const FString& GetLocalPlayerEmail() const { return Email; }
	const FString& GetSub() const { return Sub; }
	//const FString& GetLastMapName() const { return LastMapName; }
	//const FString& GetLastPlayerStartName() const { return LastPlayerStartName; }
	bool IsNewPlayer() { return bNewPlayer; }

	void KeepUserName(const FString& InUserName) { UserName = InUserName; }
	void KeepEmail(const FString& InEmail) { Email = InEmail; }
	void KeepSub(const FString& InSub) { Sub = InSub; }
	//void KeepLastMapName(const FString& MapName) { LastMapName = MapName; }
	//void KeepLastPlayerStartName(const FString& PlayerStartName) { LastPlayerStartName = PlayerStartName; }
	void KeepIsNewPlayer(bool IsNewPlayer) { bNewPlayer = IsNewPlayer; }
	
protected:
	UPROPERTY()
	FDSAuthenticationResult AuthResult;

	UPROPERTY()
	TScriptInterface<IPortalManageInterface> PortalManagementInterface;

	FString UserName;
	FString Email;
	FString Sub;
	//FString LastMapName{};
	//FString LastPlayerStartName{};

private:
	void SetRefreshTokenTimer();

	float RefreshTokenInterval = 2700.f;
	FTimerHandle RefreshTimer;

	bool bNewPlayer = false;
};

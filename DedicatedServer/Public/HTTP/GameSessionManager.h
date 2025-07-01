// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HTTP/HTTPRequestManager.h"
#include "Interfaces/GameSessionManageInterface.h"
#include "GameSessionManager.generated.h"

class UDS_GameInstanceSubsystem;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DEDICATEDSERVER_API UGameSessionManager : public UHTTPRequestManager, public IGameSessionManageInterface
{
	GENERATED_BODY()
	
public:
	void SetGameInstanceSubsystem(UDS_GameInstanceSubsystem* InSubsystem) { GameInstanceSubsystem = InSubsystem; }

	UFUNCTION()
	virtual void JoinGameSession() override;

	UFUNCTION()
	void TryCreatePlayerSession(const FString& PlayerId, const FString& GameSessionId);

	UPROPERTY(BlueprintAssignable, Category = "GameSession")
	FStatusMessageDelegate JoinStatusMessageDelegate;

protected:
	

private:
	
	void JoinGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void HandleGameSessionStatus(const FString& Status, const FString& GameSessionId);

	UPROPERTY()
	FTimerHandle CreateSessionTimer;

	UPROPERTY()
	TObjectPtr<UDS_GameInstanceSubsystem> GameInstanceSubsystem;
};

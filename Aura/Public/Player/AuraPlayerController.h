// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DedicatedServer/Public/Player/DS_PlayerController.h"
#include "Interface/OpenableInterface.h"
#include "AuraPlayerController.generated.h"

/**
 * 
 */

struct FInputActionValue;
class UAuraInputConfig;
class AAuraMagicCircle;
class UMaterialInterface;
class IHighlightInterface;
class UChatComponent;
class UInputAction;
class IChatInterface;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMouseClickedDelegate, const FHitResult&);
DECLARE_DELEGATE(FOnHudInitialized);

UCLASS()
class AURA_API AAuraPlayerController : public ADS_PlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnAbilityInputPressed(FGameplayTag InputTag);
	void OnAbilityInputReleased(FGameplayTag InputTag);
	void OnAbilityInputHeld(FGameplayTag InputTag);

	UFUNCTION()
	UChatComponent* GetChatComponent() { return ChatComponent; }

	UFUNCTION(BlueprintCallable)
	bool IsChatModeNow() const { return bChatMode; }

	// Magic Circle
	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();

	FOnMouseClickedDelegate OnMouseClickedDelegate;
	FOnHudInitialized OnHudInitialized;

	void OnRep_PlayerState();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnClientStreamingLevelLoaded() override;
	
	void Move(const FInputActionValue& InValue);

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputMappingContext> AuraInputContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ChatAction;

	void PressShift() { bShiftPressed = true; }
	void ReleaseShift() { bShiftPressed = false; }
	bool bShiftPressed = false;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UNiagaraSystem> ClickNiagaraEffect;

	UFUNCTION(BlueprintCallable)
	void UpdateMagicCircleLocation();

	UFUNCTION()
	void OnChatReceivedCallback(const FString& SenderName, const FString& Chat);

	virtual void SendMessageToAllClient(const FString& SenderName, const FString& Chat) override;

private:

	void CursorTrace();
	void AutoRun();
	bool IsConsumeItemPressed(const FGameplayTag& InputTag);
	bool IsPlayerInvincible();
	
	template<typename T>
	bool IsValidScriptInterface(const TScriptInterface<T>& Interface)
	{
		return Interface != nullptr && Interface.GetInterface() != nullptr;
	}

	UPROPERTY()
	TScriptInterface<IHighlightInterface> LastActorUnderCursor;

	UPROPERTY()
	TScriptInterface<IHighlightInterface> CurrentActorUnderCursor;

	UPROPERTY()
	TScriptInterface<IOpenableInterface> LastClickedActor;

	UPROPERTY()
	TScriptInterface<IOpenableInterface> CurrentClickedActor;
	
	

	FHitResult CursorHitResult;

	// Cached AuraASC
	UPROPERTY()
	TObjectPtr<class UAuraAbilitySystemComponent> AuraASC;

	UAuraAbilitySystemComponent* GetASC();

	// MouseHitActor Clicked
	bool IsItemOpenClick();

	// Click Move Category
	bool IsClickToMove(const FGameplayTag& InputTag);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USplineComponent> Spline;

	UPROPERTY(EditAnywhere, Category="Move")
	float AutoRunEndRadius = 50.f;

	FVector CachedDestination = FVector::ZeroVector;
	bool bAutoMove = false;
	bool bEnemyTargeting = false;
	bool bMouseHitActorTargeting = false;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	float ValidClickMouseHitActorThreshold = 200.f;

	// Magic Circle
	UPROPERTY(EditDefaultsOnly, Category="MagicCircle")
	TSubclassOf<AAuraMagicCircle> MagicCircleClass;

	UPROPERTY(VisibleAnywhere, Category="MagicCircle")
	TObjectPtr<AAuraMagicCircle> MagicCircle;

	UFUNCTION(Server, Reliable)
	void Server_SpendItem(FGameplayTag ItemTag);

	UFUNCTION(Server, Reliable)
	void Server_OpenTreasureBox(const TScriptInterface<IOpenableInterface>& ClickedActor);

	UFUNCTION(Server, Reliable)
	void Server_OnHudInitialized();

	void TryInitHUD(class AAuraPlayerState* AuraPS);

	bool bHUDInitialized = false;

	// Chat
	void ToggleChatMode();
	void ChatModeOn();
	void ChatModeOff();

	bool bChatMode = false;

	TObjectPtr<UChatComponent> ChatComponent;

};

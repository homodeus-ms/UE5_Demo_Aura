// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraUserWidget;
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
struct FWidgetControllerParams;
class UAbilitySystemComponent;
class UAttributeSet;
class USpellMenuWidgetController;
class UInventoryWidgetController;
class UChatController;

/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& Params);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& Params);
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& Params);
	UInventoryWidgetController* GetInventoryWidgetController(const FWidgetControllerParams& Params);
	UChatController* GetChatController(const FWidgetControllerParams& Params);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	void ChangeServerDefaultOverlayToAuraOverlay();
	void SetFocusToOverlay(FInputModeGameAndUI* InputMode);

	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ServerDefaultOverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> ServerDefaultOverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UChatController> ChatController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UChatController> ChatControllerClass;
};

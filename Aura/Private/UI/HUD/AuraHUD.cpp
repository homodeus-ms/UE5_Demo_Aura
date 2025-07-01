// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "UI/Widgets/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "UI/WidgetController/InventoryWidgetController.h"
#include "UI/WidgetController/ChatController.h"
#include "AuraLog.h"


void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(Aura, Log, TEXT("AuraHUD::BeginPlay()!"));
}

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params)
{
	if (OverlayWidgetController == nullptr && OverlayWidgetControllerClass != nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(Params);
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& Params)
{
	if (AttributeMenuWidgetController == nullptr && AttributeMenuWidgetControllerClass != nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(Params);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}

	return AttributeMenuWidgetController;
}

USpellMenuWidgetController* AAuraHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& Params)
{
	if (SpellMenuWidgetController == nullptr && SpellMenuWidgetControllerClass != nullptr)
	{
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(Params);
		SpellMenuWidgetController->BindCallbacksToDependencies();
	}

	return SpellMenuWidgetController;
}

UInventoryWidgetController* AAuraHUD::GetInventoryWidgetController(const FWidgetControllerParams& Params)
{
	if (InventoryWidgetController == nullptr && InventoryWidgetControllerClass != nullptr)
	{
		InventoryWidgetController = NewObject<UInventoryWidgetController>(this, InventoryWidgetControllerClass);
		InventoryWidgetController->SetWidgetControllerParams(Params);
		InventoryWidgetController->BindCallbacksToDependencies();
	}

	return InventoryWidgetController;
}

UChatController* AAuraHUD::GetChatController(const FWidgetControllerParams& Params)
{
	if (ChatController == nullptr && ChatControllerClass != nullptr)
	{
		ChatController = NewObject<UChatController>(this, ChatControllerClass);
		ChatController->SetWidgetControllerParams(Params);
		ChatController->BindCallbacksToDependencies();
	}
	return ChatController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class is NULL"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class is NULL"));
	check(ServerDefaultOverlayWidgetClass);
	UE_LOG(Aura, Log, TEXT("AuraHUD::InitOverlay()!"));

	const FWidgetControllerParams Params = FWidgetControllerParams(PC, PS, ASC, AS);

	OverlayWidgetController = GetOverlayWidgetController(Params);
	OverlayWidget = Cast<UAuraUserWidget>(CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass));
	OverlayWidget->SetWidgetController(OverlayWidgetController);

	ServerDefaultOverlayWidget = CreateWidget<UUserWidget>(GetWorld(), ServerDefaultOverlayWidgetClass);
	
	if (OverlayWidget)
	{
		OverlayWidget->AddToViewport();
		OverlayWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (ServerDefaultOverlayWidget)
	{
		ServerDefaultOverlayWidget->AddToViewport();
	}
	
	// Other WidgetControllers 'Bind Dependency' In Blueprint Using AbilitySystemLibrary 'GetController' Functions.
	// And Called BroadcastInitialValues in Blueprint
}

void AAuraHUD::ChangeServerDefaultOverlayToAuraOverlay()
{
	check(ServerDefaultOverlayWidget && OverlayWidget);

	ServerDefaultOverlayWidget->RemoveFromParent();
	OverlayWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void AAuraHUD::SetFocusToOverlay(FInputModeGameAndUI* InputMode)
{
	InputMode->SetWidgetToFocus(OverlayWidget->TakeWidget());
}

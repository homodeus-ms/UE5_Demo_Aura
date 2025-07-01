// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DedicatedServer/Public/Player/DS_LocalPlayerSubsystem.h"
#include "Interface/EnemyInterface.h"
#include "Interface/PlayerInterface.h"
#include "Interface/HighlightInterface.h"
#include "Interface/ChatInterface.h"
#include "GameplayTagContainer.h"
#include "Input/AuraInputComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SplineComponent.h"
#include "AuraGameplayTags.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "AuraLog.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Actor/AuraMagicCircle.h"
#include "Materials/MaterialInterface.h"
#include "Components/DecalComponent.h"
#include "UI/HUD/AuraHUD.h"
#include "Player/AuraPlayerState.h"
#include "Chat/ChatComponent.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	LastActorUnderCursor = nullptr;
	CurrentActorUnderCursor = nullptr;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));

	ChatComponent = CreateDefaultSubobject<UChatComponent>(TEXT("ChatComponent"));
	check(ChatComponent);
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraASC == nullptr)
	{
		AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));

	}
	return AuraASC;
}

void AAuraPlayerController::OnRep_PlayerState()
{
	UE_LOG(Aura, Log, TEXT("AAuraPlayerController, OnRep_PlayerState"));

	Super::OnRep_PlayerState();

	if (!bHUDInitialized)
	{
		AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
		check(AuraPlayerState);
		TryInitHUD(AuraPlayerState);
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraInputContext);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<class UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(AuraInputContext, 0);

		// Mouse Settings
		bShowMouseCursor = true;
		DefaultMouseCursor = EMouseCursor::Default;

		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);
	}

	if (!HasAuthority())
	{
		ChatComponent->SetUserName(UserName);
		ChatComponent->OnChatModeOff.AddLambda([this]()
			{
				if (bChatMode)
				{
					ToggleChatMode();
				}
			});

		ChatComponent->GetCurrentLevelNameDelegate.BindLambda([this]() -> FString
			{
				return GetCurrentLevelName();
			});
		
	}
	else
	{
		ChatComponent->OnChatReceivedInServer.AddUObject(this, &ThisClass::OnChatReceivedCallback);
	}

	// For Init ASC
	GetASC();
	
	if (!bHUDInitialized && IsLocalPlayerController())
	{
		UE_LOG(Aura, Log, TEXT("AuraPlayerController, Client - BeginPlay()"));

		if (AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>())
		{
			TryInitHUD(AuraPlayerState);
		}
	}
}

void AAuraPlayerController::Server_OnHudInitialized_Implementation()
{
	UE_LOG(Aura, Log, TEXT("AAuraPlayerController::Server_OnHudInitialized_Implementation()"));
	
	OnHudInitialized.ExecuteIfBound();
	SendMessageToAllClient(TEXT(""), FString::Printf(TEXT("--- %s가 게임에 들어왔습니다."), *UserName));

}

void AAuraPlayerController::TryInitHUD(AAuraPlayerState* AuraPS)
{
	if (AuraPS)
	{
		UE_LOG(Aura, Log, TEXT("AAuraPlayerController::TryInitHUD"));

		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(GetHUD()))
		{
			UAbilitySystemComponent* ASC = AuraPS->GetAbilitySystemComponent();
			check(ASC);
			UAttributeSet* AS = AuraPS->GetAttributeSet();
			check(AS);

			AuraHUD->InitOverlay(this, AuraPS, ASC, AS);

			bHUDInitialized = true;

			Server_OnHudInitialized();
		}
	}
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::PressShift);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ReleaseShift);
	AuraInputComponent->BindAction(ChatAction, ETriggerEvent::Started, this, &AAuraPlayerController::ToggleChatMode);

	AuraInputComponent->BindInputCallbacks(InputConfig, this, &ThisClass::OnAbilityInputPressed, &ThisClass::OnAbilityInputReleased, &ThisClass::OnAbilityInputHeld);
}

void AAuraPlayerController::OnClientStreamingLevelLoaded()
{
	Super::OnClientStreamingLevelLoaded();

	AAuraHUD* AuraHUD = Cast<AAuraHUD>(GetHUD());
	check(AuraHUD);
	AuraHUD->ChangeServerDefaultOverlayToAuraOverlay();
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (!bChatMode)
	{
		CursorTrace();
	}
	AutoRun();
	UpdateMagicCircleLocation();
}

void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AAuraMagicCircle>(MagicCircleClass);
		if (DecalMaterial)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
		}
	}
}
void AAuraPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}
void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(CursorHitResult.ImpactPoint);
	}
}

void AAuraPlayerController::OnChatReceivedCallback(const FString& SenderName, const FString& Chat)
{
	SendMessageToAllClient(SenderName, Chat);
}

void AAuraPlayerController::SendMessageToAllClient(const FString& SenderName, const FString& Chat)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AAuraPlayerController* PC = Cast<AAuraPlayerController>(*It);
		if (PC && PC->ChatComponent)
		{
			PC->ChatComponent->Client_BroadcastChatToWidget(SenderName, Chat);
		}
	}
}

void AAuraPlayerController::CursorTrace()
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		if (IsValidScriptInterface(CurrentActorUnderCursor))
		{
			CurrentActorUnderCursor->HighlightActor();
			CurrentActorUnderCursor = nullptr;
		}
		if (IsValidScriptInterface(LastActorUnderCursor))
		{
			LastActorUnderCursor->UnHighlightActor();
			LastActorUnderCursor = nullptr;
		}
		return;
	}

	if (!GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult))
	{
		return;
	}

	LastActorUnderCursor = CurrentActorUnderCursor;
	CurrentActorUnderCursor = CursorHitResult.GetActor();

	bEnemyTargeting = CursorHitResult.GetActor()->Implements<UEnemyInterface>() ? true : false;

	if (LastActorUnderCursor == CurrentActorUnderCursor)
	{
		return;
	}

	if (IsValidScriptInterface(CurrentActorUnderCursor))
	{
		CurrentActorUnderCursor->HighlightActor();
	}
	if (IsValidScriptInterface(LastActorUnderCursor))
	{
		LastActorUnderCursor->UnHighlightActor();
	}
}

void AAuraPlayerController::AutoRun()
{
	APawn* ControlledPawn = GetPawn();
	if (bAutoMove && ControlledPawn)
	{
		const FVector NextLocation = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(NextLocation, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistToNextLocation = (NextLocation - CachedDestination).Length();
		if (DistToNextLocation <= AutoRunEndRadius)
		{
			bAutoMove = false;
		}
	}
}

bool AAuraPlayerController::IsConsumeItemPressed(const FGameplayTag& InputTag)
{
	return InputTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Input.Consume")));
}

bool AAuraPlayerController::IsPlayerInvincible()
{
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetPawn()))
	{
		return PlayerInterface->IsInvincible();
	}

	return false;
}

void AAuraPlayerController::OnAbilityInputPressed(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
		return;

	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_Action_LMB))
	{
		if (CursorHitResult.GetActor() && CursorHitResult.GetActor()->Implements<UOpenableInterface>())
		{
			CurrentClickedActor = CursorHitResult.GetActor();
			bMouseHitActorTargeting = true;

			if (LastClickedActor == CurrentClickedActor)
			{
				if (IsItemOpenClick())
				{
					IOpenableInterface::Execute_ShowBoxOpenEffect(CurrentClickedActor.GetObject());
					Server_OpenTreasureBox(CurrentClickedActor);
				}
			}
			else
			{
				if (IsValidScriptInterface(LastClickedActor))
				{
					LastClickedActor->OnDeselected();
				}

				if (IsValidScriptInterface(CurrentClickedActor))
				{
					CurrentClickedActor->OnSelected();
				}

				LastClickedActor = CurrentClickedActor;
			}

			return;
		}
		else
		{
			bMouseHitActorTargeting = false;
		}
	}
	else if (IsConsumeItemPressed(InputTag))
	{
		if (HasAuthority())
		{
			UE_LOG(Aura, Warning, TEXT("This Should Not Called in Server, %s"), *FString(__FUNCTION__));
		}
		else
		{
			// 서버에 Spend 요청 보내기
			Server_SpendItem(InputTag);
		}
	}
	
	if (!IsPlayerInvincible())
		GetASC()->ActivateAbilityOnPressed(InputTag);
}

void AAuraPlayerController::OnAbilityInputReleased(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased))
		return;

	if (bMouseHitActorTargeting && IsItemOpenClick())
	{
		return;
	}

	if (!IsClickToMove(InputTag))
	{ 
		UE_LOG(Aura, Log, TEXT("!IsClickToMove"));
		AuraASC->DeActivateAbilityOnReleased(InputTag);
		return;
	}

	APawn* ControlledPawn = GetPawn();
	if (FollowTime < ShortPressThreshold && ControlledPawn)
	{
		// For Auto Move To Check Point Specific Location
		if (CurrentActorUnderCursor && !CurrentActorUnderCursor.GetObject()->Implements<UEnemyInterface>())
		{
			CurrentActorUnderCursor->SetDestinationOnClicked(CachedDestination);
		}

		if (const UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, ControlledPawn->GetActorLocation(), CachedDestination))
		{
			Spline->ClearSplinePoints();
			for (const FVector& Point : Path->PathPoints)
			{
				Spline->AddSplinePoint(Point, ESplineCoordinateSpace::World);
			}

			bAutoMove = true;

			int32 LastIdx = Path->PathPoints.Num() - 1;
			if (Path->PathPoints.IsValidIndex(LastIdx))
				CachedDestination = Path->PathPoints[Path->PathPoints.Num() - 1];
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FindPathToLocationSynchronously failed."));
			
			UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
			if (NavSys)
			{
				FNavLocation OutStart, OutEnd;
				bool bStartProjected = NavSys->ProjectPointToNavigation(ControlledPawn->GetActorLocation(), OutStart);
				bool bEndProjected = NavSys->ProjectPointToNavigation(CachedDestination, OutEnd);

				UE_LOG(LogTemp, Warning, TEXT("Start on NavMesh: %d | End on NavMesh: %d"), bStartProjected, bEndProjected);

			}
		}

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraEffect, CachedDestination);
	}
	
	FollowTime = 0.f;
	bEnemyTargeting = false;
}

void AAuraPlayerController::OnAbilityInputHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputHeld))
		return;

	if (!IsClickToMove(InputTag))
	{
		if (!IsPlayerInvincible())
			AuraASC->ActivateAbilityOnHeld(InputTag);

		bAutoMove = false;
		return;
	}
	
	FollowTime += GetWorld()->GetDeltaSeconds();

	if (CursorHitResult.bBlockingHit)
	{
		CachedDestination = CursorHitResult.ImpactPoint;
	}

	if (APawn* ControlledPawn = GetPawn())
	{
		FVector Direction = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(Direction * FollowTime);
	}
}

void AAuraPlayerController::Move(const FInputActionValue& InValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
		return;

	bAutoMove = false;

	const FVector2D MoveVector = InValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);
	
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(Forward, MoveVector.X);
		ControlledPawn->AddMovementInput(Right, MoveVector.Y);
	}
}

bool AAuraPlayerController::IsClickToMove(const FGameplayTag& InputTag)
{
	return !bShiftPressed && !bEnemyTargeting &&
		InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_Action_LMB);
}


void AAuraPlayerController::Server_OpenTreasureBox_Implementation(const TScriptInterface<IOpenableInterface>& ClickedActor)
{
	check(ClickedActor);
	UE_LOG(Aura, Log, TEXT("Server OpenTreasuer Box!, %s"), *ClickedActor.GetObject()->GetName());
	ClickedActor->OpenBox(GetPawn());
}


bool AAuraPlayerController::IsItemOpenClick()
{
	if (!bMouseHitActorTargeting)
		return false;

	const FVector PlayerLocation = GetPawn()->GetActorLocation();
	const FVector& HitPoint = CursorHitResult.ImpactPoint;

	float Distance = (HitPoint - PlayerLocation).Length();

	return Distance < ValidClickMouseHitActorThreshold;
}

void AAuraPlayerController::Server_SpendItem_Implementation(FGameplayTag ItemTag)
{
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetPawn()))
		PlayerInterface->SpendItem(ItemTag);
}

void AAuraPlayerController::ToggleChatMode()
{
	if (!ChatComponent)
	{
		UE_LOG(Aura, Log, TEXT("AAuraPlayerController, ChatComponent Is NULL"));
		return;
	}

	if (bChatMode)
	{
		ChatModeOff();
	}
	else
	{
		ChatModeOn();
	}
}

void AAuraPlayerController::ChatModeOn()
{
	UE_LOG(Aura, Log, TEXT("bChatMode False->True"));
	bChatMode = true;
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	ChatComponent->SetChatModeOn();
}

void AAuraPlayerController::ChatModeOff()
{
	UE_LOG(Aura, Log, TEXT("bChatMode True->False"));
	bChatMode = false;
	FInputModeGameAndUI InputMode;
	AAuraHUD* AuraHUD = Cast<AAuraHUD>(GetHUD());
	AuraHUD->SetFocusToOverlay(&InputMode);
	
	SetInputMode(InputMode);
}

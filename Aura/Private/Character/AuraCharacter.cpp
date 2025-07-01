// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraCharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Player/AuraPlayerState.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/Widgets/DamageTextWidgetComponent.h"
#include "AuraLog.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/NiagaraComponents/AuraNiagaraComponent.h"
#include "AbilitySystem/NiagaraComponents/PassiveSkillNiagaraComponent.h"
#include "AuraGameplayTags.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
// TODO : Remove this
#include "SaveGame/LoadScreenSaveGame.h"
#include "DedicatedServer/Public/Game/DS_GameInstanceSubsystem.h"
#include "DedicatedServer/Public/Interfaces/SaveManageInterface.h"
#include "DedicatedServer/Public/Interfaces/GameSessionManageInterface.h"
#include "Save/AuraSaveData.h"
#include "Save/SaveHelper.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AuraGameUtility.h"
#include "Interface/StageClearObserver.h"

AAuraCharacter::AAuraCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UAuraCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{

	RootComponent = GetCapsuleComponent();

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 800.f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm);

	NameTagWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameTag"));
	NameTagWidgetComponent->SetupAttachment(RootComponent);
	NameTagWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f)); // 머리 위로 위치 조정
	NameTagWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // 또는 World
	NameTagWidgetComponent->SetDrawSize(FVector2D(150.f, 50.f));
	NameTagWidgetComponent->SetWidgetClass(NameTagWidgetClass);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	LevelUpEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelUpNiagaraEffect"));
	LevelUpEffect->SetupAttachment(RootComponent);
	LevelUpEffect->bAutoActivate = false;

	CharacterType = ECharacterType::Aura;
	BaseWalkSpeed = 600.f;

	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

	HaloOfProtectionEffectComponent = CreateDefaultSubobject<UPassiveSkillNiagaraComponent>(TEXT("HaloOfProtectionEffectComponent"));
	HaloOfProtectionEffectComponent->SetupAttachment(RootComponent);
	HaloOfProtectionEffectComponent->NiagaraTag = AuraTags.Abilities_Passive_HaloOfProtection;
	HaloOfProtectionEffectComponent->SetRelativeLocation({ 0.f, 0.f, -20.f });

	HealthSiphonEffectComponent = CreateDefaultSubobject<UPassiveSkillNiagaraComponent>(TEXT("HealthSiphonEffectComponent"));
	HealthSiphonEffectComponent->SetupAttachment(RootComponent);
	HealthSiphonEffectComponent->NiagaraTag = AuraTags.Abilities_Passive_HealthSiphon;
	HealthSiphonEffectComponent->SetRelativeLocation({ 0.f, 0.f, -20.f });

	ManaSiphonEffectComponent = CreateDefaultSubobject<UPassiveSkillNiagaraComponent>(TEXT("ManaSiphonEffectComponent"));
	ManaSiphonEffectComponent->SetupAttachment(RootComponent);
	ManaSiphonEffectComponent->NiagaraTag = AuraTags.Abilities_Passive_ManaSiphon;
	ManaSiphonEffectComponent->SetRelativeLocation({ 0.f, 0.f, -20.f });
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
		UE_LOG(Aura, Log, TEXT("--- AAuraCharacter::BeginPlay()"));

	if (HasAuthority())
	{
		bIsInvincible = true;
		FTimerDelegate OnSpawnProtectionDelegate;
		OnSpawnProtectionDelegate.BindLambda([this]()
			{
				UE_LOG(Aura, Log, TEXT("bIsInvincible: True -> False"));
				bIsInvincible = false;
			});

		GetWorld()->GetTimerManager().SetTimer(
			OnSpawnProtectionTimer, OnSpawnProtectionDelegate, OnSpawnProtectionTime, false);
	}
	
}

void AAuraCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraCharacter, bIsInvincible);
}

void AAuraCharacter::OnRep_PlayerState()
{
	UE_LOG(Aura, Log, TEXT("--- AAuraCharacter::OnRep_PlayerState()"));
	InitAbilitySystem();

	// For Blueprint
	OnPlayerStateReplicated();
}

void AAuraCharacter::AddToXP(int32 XP)
{
	GetAuraPlayerState()->AddXP(XP);
}

void AAuraCharacter::AddGold(int32 Gold)
{
	GetAuraPlayerState()->AddGold(Gold);
}

void AAuraCharacter::GetItem(const FGameplayTag& ItemSpendTag, int32 Amount)
{
	GetAuraPlayerState()->AddInventoryItemByItemSpendTag(ItemSpendTag, Amount);
}

void AAuraCharacter::AddToSpellPoint(int32 AddPoint)
{
	GetAuraPlayerState()->AddSpellPoint(AddPoint);
}

void AAuraCharacter::UpdateLevel(int32 NewLevel)
{
	GetAuraPlayerState()->UpdateLevel(NewLevel);
}

void AAuraCharacter::UpdateAttributePoint(int32 Value)
{
	GetAuraPlayerState()->AddAttributePoint(Value);
}

AAuraPlayerController* AAuraCharacter::GetAuraPlayerController() const
{
	return Cast<AAuraPlayerController>(GetController());
}

AAuraPlayerState* AAuraCharacter::GetAuraPlayerState()
{
	if (AuraPS)
		return AuraPS;

	AuraPS = Cast<AAuraPlayerState>(GetPlayerState());
	return AuraPS;
}

int32 AAuraCharacter::GetNewLevelForXP(int32 XP)
{
	return GetAuraPlayerState()->GetNewLevelForXP(XP);
}

int32 AAuraCharacter::GetCurrentXP()
{
	return GetAuraPlayerState()->GetXP();
}

int32 AAuraCharacter::GetCurrentGold()
{
	return GetAuraPlayerState()->GetGold();
}

int32 AAuraCharacter::GetAttributePoint()
{
	return GetAuraPlayerState()->GetAttributePoint();
}

int32 AAuraCharacter::GetSpellPoint()
{
	return GetAuraPlayerState()->GetSpellPoint();
}

void AAuraCharacter::SpendItem(const FGameplayTag& ItemTag)
{
	GetAuraPlayerState()->SpendItem(ItemTag);
}

void AAuraCharacter::RecieveBoxItem(const FGameplayTag& ItemTag, int32 ItemAmount)
{
	if (!HasAuthority())
	{
		UE_LOG(Aura, Log, TEXT("Called In Client!, %s"), *FString(__FUNCTION__));
		return;
	}

	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

	// TODO : CleanUp Here
	if (ItemTag.MatchesTag(FGameplayTag::RequestGameplayTag("Item.Buyable")))
	{
		GetAuraPlayerState()->AddInventoryItemFromTreasureBox(ItemTag, ItemAmount);
	}
	else if (ItemTag.MatchesTagExact(AuraTags.Item_Gold))
	{
		GetAuraPlayerState()->AddInventoryItemFromTreasureBox(ItemTag, ItemAmount);
	}
	else if (ItemTag.MatchesTagExact(AuraTags.Item_Vacant))
	{
		
	}
	else
	{
		GetAuraPlayerState()->AddCollectingItem(ItemTag);
	}
}

bool AAuraCharacter::IsSiphonEffectActivate()
{
	return IsHealthSiphonActive() || IsManaSiphonActive();
}

bool AAuraCharacter::IsLevelUpCondition(int32 InXP)
{
	return GetAuraPlayerState()->IsLevelUpCondition(InXP);
}

void AAuraCharacter::HandleLevelUp()
{
	MulticastShowLevelUpEffect();
}



bool AAuraCharacter::IsHaloOfProtectionActive()
{
	return HaloOfProtectionEffectComponent->IsSkillActive();
}

bool AAuraCharacter::IsHealthSiphonActive()
{
	return HealthSiphonEffectComponent->IsSkillActive();
}

bool AAuraCharacter::IsManaSiphonActive()
{
	return ManaSiphonEffectComponent->IsSkillActive();
}

void AAuraCharacter::HandleSiphonEffect(float InDamage)
{
	const float SiphonAmount = InDamage / 4.f;
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

	if (IsHealthSiphonActive())
	{
		FGameplayEffectContextHandle NewContext = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(HealthSiphonEffectClass, 1.f, NewContext);
		SpecHandle.Data->SetSetByCallerMagnitude(AuraTags.Attribute_Vital_Health, SiphonAmount);
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}

	if (IsManaSiphonActive())
	{
		FGameplayEffectContextHandle NewContext = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(ManaSiphonEffectClass, 1.f, NewContext);
		SpecHandle.Data->SetSetByCallerMagnitude(AuraTags.Attribute_Vital_Mana, SiphonAmount);
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(GetController()))
	{
		PC->ShowMagicCircle(DecalMaterial);
	}
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(GetController()))
	{
		PC->HideMagicCircle();
	}
}


void AAuraCharacter::SaveCurrentGame(const FString& LevelName, const FName& PlayerStartName)
{
	if (!HasAuthority())
		return;

	AAuraPlayerState* PS = GetAuraPlayerState();
	check(PS);
	UAuraSaveData* SaveData = PS->GetSaveData();
	check(SaveData);

	UE_LOG(Aura, Log, TEXT("----- Chacracter, SaveCurrentGame -----"));
	UE_LOG(Aura, Log, TEXT("PS Ptr: %p"), PS);
	UE_LOG(Aura, Log, TEXT("SaveData Ptr: %p"), SaveData);

	SaveData->PlayerData.MapName = LevelName;
	SaveData->PlayerData.PlayerStart = PlayerStartName.ToString();

	// Save Stat
	if (GetAuraPlayerState())
	{
		SaveData->PlayerData.PlayerLevel = PS->GetPlayerLevel();
		SaveData->PlayerData.Xp = PS->GetXP();
		SaveData->PlayerData.AttributePoint = PS->GetAttributePoint();
		SaveData->PlayerData.SpellPoint = PS->GetSpellPoint();
	}
	SaveData->AttributesData.Strength = UAuraAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
	SaveData->AttributesData.Intelligence = UAuraAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
	SaveData->AttributesData.Resilience = UAuraAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
	SaveData->AttributesData.Vigor = UAuraAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());
	SaveData->AttributesData.CurrentHP = UAuraAttributeSet::GetHealthAttribute().GetNumericValue(GetAttributeSet());
	SaveData->AttributesData.CurrentMP = UAuraAttributeSet::GetManaAttribute().GetNumericValue(GetAttributeSet());
	
	// Inventory
	SaveData->InventoryData.Gold = GetCurrentGold();
	FSaveHelper::ConvertTagMapToStringMap(PS->GetInventoryItems(), SaveData->InventoryData.InventoryItemAmounts);
	FSaveHelper::ConvertTagArrayToStringArray(PS->GetCollections(), SaveData->InventoryData.Collections);
	
	// Skills
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(ASC);
	
	FGetEachAbilitySpecFuncType SpecFunc;
	SpecFunc.BindLambda([this, AuraASC, SaveData](const FGameplayAbilitySpec& AbilitySpec)
		{
			const FGameplayTag& AbilityTag = AuraASC->GetAbilityTagFromAbilitySpec(AbilitySpec);

			if (AbilityTag.IsValid())
			{
				UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(this);
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityByAbilityTag(AbilityTag);

				FSkillSaveData SkillSaveData;

				SkillSaveData.AbilityClassPath = UAuraAbilitySystemLibrary::ConvertClassToStringPath(Info.AbilityClass);
				SkillSaveData.AbilityLevel = AbilitySpec.Level;
				SkillSaveData.AbilitySlot = AuraASC->GetSlotTagFromAbilitySpec(AbilitySpec).ToString();
				SkillSaveData.AbilityStatus = AuraASC->GetStatusTagFromAbilitySpec(AbilitySpec).ToString();
				SkillSaveData.AbilityTag = AbilityTag.ToString();
				SkillSaveData.AbilityType = Info.TypeTag.ToString();

				int32 BeforeArraySize = SaveData->SavedSkills.Num();
				int32 Index = SaveData->SavedSkills.AddUnique(SkillSaveData);
				 
				if (BeforeArraySize == SaveData->SavedSkills.Num()) // Already Have Item
				{
					UE_LOG(Aura, Log, TEXT("[InLambda, Overwrite] Before Level: %d, After Level: %d"),
						SaveData->SavedSkills[Index].AbilityLevel, SkillSaveData.AbilityLevel);
					SaveData->SavedSkills[Index] = SkillSaveData; // Overwrite it
				} 
				else
				{
					UE_LOG(Aura, Log, TEXT("[InLambda, Overwrite] %s Just Added!"), *AbilityTag.ToString());
				}
			}
		});

	AuraASC->GetEachAbilitySpec(SpecFunc);

	if (ISaveManageInterface* SaveManageInterface = PS->GetSaveManageInterface())
	{
		SaveManageInterface->SaveGame(PS);
	}
}

void AAuraCharacter::LoadPlayerData(bool IsFirstLoadIn)
{
	UE_LOG(Aura, Log, TEXT("=== AAuraCharacter::LoadPlayerData() ==="));

	if (!HasAuthority())
		return;

	UAuraSaveData* SaveData = GetAuraPlayerState()->GetSaveData();
	if (!SaveData)
		return;

	/*TScriptInterface<IStageClearObserver> StageClearObserver = UAuraGameUtility::GetStageClearObserver(this);
	if (!StageClearObserver)
		return;
	StageClearObserver->SetMapInfo();*/

	if (IsFirstLoadIn)
	{
		UAuraAbilitySystemLibrary::InitDefaultAttributes(this, ASC, GetCharacterType(), 1.f);
		UAuraAbilitySystemLibrary::InitCommonAbilities(this, ASC);
		UAuraAbilitySystemLibrary::InitStartupAbilities(this, ASC, CharacterType, GetCharacterLevel());
		UAuraAbilitySystemLibrary::InitPassiveAbilities(this, ASC, PassiveAbilities);
		
		if (AuraPS)
		{
			AuraPS->SetPlayerLevelByLoadedData(1);
		}
		
		return;
	}
	
	const FPlayerInfoSaveData& PlayerInfoData = SaveData->PlayerData;
	const FAttributesSaveData& AttributeData = SaveData->AttributesData;
	const FInventorySaveData& InventoryData = SaveData->InventoryData;

	if (AuraPS)
	{
		AuraPS->SetPlayerLevelByLoadedData(PlayerInfoData.PlayerLevel);
		AuraPS->SetXPByLoadedData(PlayerInfoData.Xp);
		AuraPS->AddAttributePoint(PlayerInfoData.AttributePoint);
		AuraPS->AddSpellPoint(PlayerInfoData.SpellPoint);
	}
	UE_LOG(Aura, Log, TEXT("Level: %d"), PlayerInfoData.PlayerLevel);
	UE_LOG(Aura, Log, TEXT("XP: %d"), PlayerInfoData.Xp);
	UE_LOG(Aura, Log, TEXT("AttributePoint: %d"), PlayerInfoData.AttributePoint);
	UE_LOG(Aura, Log, TEXT("SpellPoint: %d"), PlayerInfoData.SpellPoint);

	// Attribute
	UAuraAbilitySystemLibrary::InitLoadedAttributes(this, ASC, AttributeData);

	// CurrentHP, MP
	if (UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(GetAttributeSet()))
	{
		AuraAS->SetHealth(AttributeData.CurrentHP);
		AuraAS->SetMana(AttributeData.CurrentMP);
	}

	// Inventory
	TMap<FGameplayTag, int32> Items;
	FSaveHelper::ConvertStringMapToTagMap(InventoryData.InventoryItemAmounts, Items);
	TArray<FGameplayTag> Collections;
	FSaveHelper::ConvertStringArrayToTagArray(InventoryData.Collections, Collections);

	AuraPS->SetInventoryItems(Items);
	AuraPS->SetCollectionsByLoadedData(Collections);
	AuraPS->SetGoldByLoadedData(InventoryData.Gold);

	// Skills
	Cast<UAuraAbilitySystemComponent>(ASC)->AddPlayerAbilitiesFromLoadedData(SaveData);
}

uint8 AAuraCharacter::GetCharacterLevel()
{
	return GetAuraPlayerState()->GetPlayerLevel();
}

void AAuraCharacter::Die()
{
	UE_LOG(Aura, Log, TEXT("AAuraCharacter::Die()"));
	if (!HasAuthority())
		return;

	Super::Die();

	ASC->AddLooseGameplayTags(BlockInputTagsContainer);

	// Client RPC
	ShowDeadMessage();

	FTimerDelegate DeadDelegate;
	DeadDelegate.BindLambda([this]()
		{
			UE_LOG(Aura, Log, TEXT("Character, In DeadDelegate Lambda, Call ReSpawn Client RPC()"));
			//Client RPC
			ReSpawn();
		});
	GetWorld()->GetTimerManager().SetTimer(DeadTimer, DeadDelegate, DeadDelayTime, false);
	FollowCamera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}


void AAuraCharacter::ShowDamageText_Implementation(float DamageNum, bool bBlocked, bool bCritical, ACharacter* Target)
{
	if (!FlowtingTextWidgetClass)
	{
		UE_LOG(Aura, Warning, TEXT("Flowting Text Widget Class Is Null"));
		return;
	}
	if (!IsValid(Target))
	{
		UE_LOG(Aura, Warning, TEXT("Target Is NULL Or Target Is Pending Kill"), __FUNCTION__, __LINE__);
		return;
	}

	UDamageTextWidgetComponent* FlowtingTextWidget = NewObject<UDamageTextWidgetComponent>(this, FlowtingTextWidgetClass);
	FlowtingTextWidget->RegisterComponent();

	// Target에 붙였다가 이후에는 자체 애니메이션으로 움직이므로 바로 Detach
	FlowtingTextWidget->AttachToComponent(Target->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	FlowtingTextWidget->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	FlowtingTextWidget->SetDamageText(DamageNum, bBlocked, bCritical);
}

void AAuraCharacter::InitAbilitySystem()
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	
	if (AuraPlayerState)
	{
		ASC = AuraPlayerState->GetAbilitySystemComponent();
		AttributeSet = AuraPlayerState->GetAttributeSet();
		ASC->InitAbilityActorInfo(AuraPlayerState, this);
		UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(ASC);
		AuraASC->AbilityActorInfoSet();
	}

	/*if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, ASC, AttributeSet);
		}
	}*/

	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

	ASC->RegisterGameplayTagEvent(AuraTags.Debuff_Stun).AddUObject(this, &ThisClass::StunTagChanged);

	BlockInputTagsContainer.AddTag(AuraTags.Player_Block_CursorTrace);
	BlockInputTagsContainer.AddTag(AuraTags.Player_Block_InputHeld);
	BlockInputTagsContainer.AddTag(AuraTags.Player_Block_InputPressed);
	BlockInputTagsContainer.AddTag(AuraTags.Player_Block_InputReleased);

	OnASCRegistered.Broadcast(ASC);
}

void AAuraCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);

	if (bIsStunned)
	{
		AddBlockInputLooseTagsWhenStunned();
	}
	else
	{
		RemoveBlockInputLooseTagsRecoverStunned();
	}
}

void AAuraCharacter::OnRep_Stunned()
{
	if (bIsStunned)
	{
		AddBlockInputLooseTagsWhenStunned();
	}
	else
	{
		RemoveBlockInputLooseTagsRecoverStunned();
	}
}

void AAuraCharacter::AddBlockInputLooseTagsWhenStunned()
{
	ASC->AddLooseGameplayTags(BlockInputTagsContainer);
}

void AAuraCharacter::RemoveBlockInputLooseTagsRecoverStunned()
{
	ASC->RemoveLooseGameplayTags(BlockInputTagsContainer);
}

void AAuraCharacter::ShowDeadMessage_Implementation()
{
	if (DeadMessageWidgetClass)
	{
		if (UUserWidget* Message = CreateWidget<UUserWidget>(GetWorld(), DeadMessageWidgetClass))
		{
			UE_LOG(Aura, Log, TEXT("Dead Message Add to Viewport!"));
			Message->AddToViewport();
		}
	}
}

void AAuraCharacter::ReSpawn_Implementation()
{
	if (UDS_GameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UDS_GameInstanceSubsystem>())
	{
		Subsystem->SetIsRespawn(true);

		if (IGameSessionManageInterface* Interface = Subsystem->GetGameSessionManageInterface())
		{
			UE_LOG(Aura, Log, TEXT("Call Join Session() By Using Interface"));
			Interface->JoinGameSession();
		}
	}
	else
	{
		UE_LOG(Aura, Log, TEXT("Subsystem Is NOT Valid, %s"), LOG_POS);
	}
}

void AAuraCharacter::MulticastShowLevelUpEffect_Implementation() const
{
	if (IsValid(LevelUpEffect))
	{
		const FVector CameraLocation = FollowCamera->GetComponentLocation();
		const FVector EffectLocation = LevelUpEffect->GetComponentLocation();
		const FRotator Rotation = (CameraLocation - EffectLocation).Rotation();
		LevelUpEffect->SetWorldRotation(Rotation);

		LevelUpEffect->Activate(true);
	}
}



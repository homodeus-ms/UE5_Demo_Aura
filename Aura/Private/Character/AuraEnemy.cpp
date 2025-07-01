// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AuraDefines.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "UI/Widgets/AuraUserWidget.h"
#include "UI/WidgetController/EnemyWidgetController.h"
#include "AuraGameplayTags.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI/AuraAIDefines.h"
#include "Interface/StageClearObserver.h"
#include "Interface/PlayerInterface.h"
#include "AuraGameUtility.h"


AAuraEnemy::AAuraEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	ASC = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));

	check(AttributeSet);

	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	HpBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHpBar"));
	HpBarComponent->SetupAttachment(RootComponent);
	HpBarComponent->SetRelativeLocation(FVector(0.f, 0.f, 180.f));

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	GetMesh()->MarkRenderStateDirty();
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->MarkRenderStateDirty();
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
	
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

uint8 AAuraEnemy::GetCharacterLevel()
{
	return CharacterLevel;
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::InitCommonAbilities(this, ASC);
		UAuraAbilitySystemLibrary::InitStartupAbilities(this, ASC, CharacterType, GetCharacterLevel());
	}

	AIController = Cast<AAuraAIController>(NewController);
	AIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->GetBlackboardAsset());
	AIController->RunBehaviorTree(BehaviorTree);
	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_bRANGED_UNIT, CharacterType != ECharacterType::Warrior);
}


int32 AAuraEnemy::GetRewardPoint() const
{
	return UAuraAbilitySystemLibrary::GetRewardPoint(this, CharacterType, CharacterLevel);
}

bool AAuraEnemy::IsTargetInAttackRange(AActor* InTargetActor) const
{
	if (!IsValid(TargetActor))
		return false;

	const FVector MyLocation = GetActorLocation();
	const FVector TargetLocation = InTargetActor->GetActorLocation();

	return (MyLocation - TargetLocation).Length() <= AttackRange;
}

void AAuraEnemy::AddMinionCount(int32 Count)
{
	MinionCount += Count;
	AIController->GetBlackboardComponent()->SetValueAsInt(BBKEY_LIVE_MINION_COUNT, MinionCount);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitDefaultAttribute();

	if (!HasAuthority())
		InitHpBarWidget();
}

void AAuraEnemy::InitAbilitySystem()
{
	ASC->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(ASC)->AbilityActorInfoSet();
	InitDefaultAttribute();

	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	ASC->RegisterGameplayTagEvent(AuraTags.Debuff_Stun).AddUObject(this, &ThisClass::StunTagChanged);
	
	OnASCRegistered.Broadcast(ASC);
}

void AAuraEnemy::Die()
{
	if (TScriptInterface<IStageClearObserver> StageClearObserver = UAuraGameUtility::GetStageClearObserver(this))
	{
		if (!IsSpawnBySummon())
			StageClearObserver->OnEnemyKilled();
	}
	else
	{
		UE_LOG(Aura, Warning, TEXT("StageClearObserver Is Null!!"));
	}

	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_bDEAD, true);
	SetLifeSpan(DisappearDelayTime);
	
	Super::Die();
}

void AAuraEnemy::InitDefaultAttribute() const
{
	UAuraAbilitySystemLibrary::InitDefaultAttributes(this, ASC, CharacterType, CharacterLevel);
}



void AAuraEnemy::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);

	if (AIController && AIController->GetBlackboardComponent())
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_bSTUNNED, bIsStunned);
	}
}

void AAuraEnemy::InitHpBarWidget()
{
	UAuraUserWidget* HpBar = CastChecked<UAuraUserWidget>(HpBarComponent->GetWidget());
	EnemyWidgetController = NewObject<UEnemyWidgetController>(this, WidgetControllerClass);
	
	FWidgetControllerParams Params(nullptr, nullptr, ASC, AttributeSet);
	EnemyWidgetController->SetWidgetControllerParams(Params);
	HpBar->SetWidgetController(EnemyWidgetController);

	EnemyWidgetController->BroadcastInitialValues();
	EnemyWidgetController->BindCallbacksToDependencies();

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interface/EnemyInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AuraGameplayTags.h"
#include "AuraLog.h"
#include "AI/AuraAIDefines.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystem/NiagaraComponents/AuraNiagaraComponent.h"
#include "AbilitySystem/NiagaraComponents/DebuffNiagaraComponent.h"
#include "Net/UnrealNetwork.h"

AAuraCharacterBase::AAuraCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	RootComponent = GetCapsuleComponent();

	// Debuff Niagara Effect Component Set up

	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	
	BurnDebuffEffectComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>(TEXT("BurnDebuffComponent"));
	BurnDebuffEffectComponent->SetupAttachment(RootComponent);
	BurnDebuffEffectComponent->NiagaraTag = AuraTags.Debuff_Burn;
	BurnDebuffEffectComponent->SetRelativeLocation(FVector(0.f, 0.f, -20.f));

	StunDebuffEffectComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>(TEXT("StunDebuffComponent"));
	StunDebuffEffectComponent->SetupAttachment(RootComponent);
	StunDebuffEffectComponent->NiagaraTag = AuraTags.Debuff_Stun;
	StunDebuffEffectComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

}

void AAuraCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraCharacterBase, bIsStunned);
	DOREPLIFETIME(AAuraCharacterBase, bIsShockLoop);
}

void AAuraCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilitySystem();
}

void AAuraCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (ASC)
		ASC->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact).AddUObject(this, &ThisClass::OnHitReactTagChanged);

}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

USkeletalMeshComponent* AAuraCharacterBase::GetWeaponComponent_Implementation() const
{
	return Weapon;
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) const
{
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	if (!CombatSockets.Contains(MontageTag))
	{
		UE_LOG(Aura, Warning, TEXT("No Combat Socket Tags, %s, %d"), __FUNCTION__, __LINE__);
		return FVector();
	}

	FName SocketName = CombatSockets[MontageTag];

	if (MontageTag.MatchesTagExact(AuraTags.CombatSocket_Weapon))
	{
		return Weapon->GetSocketLocation(SocketName);
	}
	else
	{
		return GetMesh()->GetSocketLocation(SocketName);
	}
}

FTaggedMontage AAuraCharacterBase::GetAttackMontageFromArray_Implementation() const
{
	if (AttackMontages.Num() == 0)
	{
		UE_LOG(Aura, Error, TEXT("No Attack Montage!"));
		static FTaggedMontage Dummy;
		return Dummy;
	}

	if (AttackMontages.Num() == 1)
		return AttackMontages[0];

	int32 RandomIdx = FMath::RandRange(0, AttackMontages.Num() - 1);

	return AttackMontages[RandomIdx];
}

FTaggedMontage AAuraCharacterBase::GetAttackMontageByMontageTag_Implementation(FGameplayTag MontageTag) const
{
	for (const FTaggedMontage& TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.AttackMontageTag.MatchesTagExact(MontageTag))
		{
			return TaggedMontage;
		}
	}

	UE_LOG(Aura, Error, TEXT("No Attack Montage!"));
	static FTaggedMontage Dummy;

	return Dummy;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation() const
{
	return HitReaction;
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation() const
{
	return BloodEffect;
}

void AAuraCharacterBase::Die()
{
	UE_LOG(Aura, Log, TEXT("AAuraCharacterBase::Die()"));
	bDead = true;
	
	ASC->CancelAllAbilities();
	RemoveDebuffsOnDead();
	MulticastHandleDeath();
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

bool AAuraCharacterBase::IsFriend_Implementation(const AActor* OtherActor) const
{
	bool thisHasPlayerTag = ActorHasTag(FName(TEXT("Player")));
	bool OtherHasEnemyTag = OtherActor->ActorHasTag(FName(TEXT("Enemy")));
	
	// T, T -> Player, Enemy -> F
	// T, F -> Player, Player -> T
	// F, T -> Enemy, Enemy -> T
	// F, F -> Enemy, Player -> F
	
	return (thisHasPlayerTag != OtherHasEnemyTag);
}

void AAuraCharacterBase::SetShockLoop_Implementation(bool bLooping)
{
	bIsShockLoop = bLooping;
}

bool AAuraCharacterBase::IsShockLoop_Implementation() const
{
	return bIsShockLoop;
}

void AAuraCharacterBase::Dissolve()
{
	if (CharacterDissolveMaterial)
	{
		UMaterialInstanceDynamic* MaterialInst = 
			UMaterialInstanceDynamic::Create(CharacterDissolveMaterial, this);
		GetMesh()->SetMaterial(0, MaterialInst);
		StartDissolveCharacter(MaterialInst);
	}
	if (WeaponDissolveMaterial)
	{
		UMaterialInstanceDynamic* MaterialInst =
			UMaterialInstanceDynamic::Create(WeaponDissolveMaterial, this);
		Weapon->SetMaterial(0, MaterialInst);
		StartDissolveWeapon(MaterialInst);
	}
}
void AAuraCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;

	if (bIsStunned)
		StunDebuffEffectComponent->Activate();
	else
		StunDebuffEffectComponent->Deactivate();
}

void AAuraCharacterBase::RemoveDebuffsOnDead()
{
	const FGameplayTagContainer DebuffParentTag(FAuraGameplayTags::Get().Debuff);
	FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(DebuffParentTag);

	TArray<FActiveGameplayEffectHandle> ActiveEffects = ASC->GetActiveEffects(Query);
	for (const FActiveGameplayEffectHandle& EffectHandle : ActiveEffects)
	{
		ASC->RemoveActiveGameplayEffect(EffectHandle);
	}
}


void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	OnDeadDelegate.Broadcast(this);

	// HpBar
	if (IEnemyInterface* Enemy = Cast<IEnemyInterface>(this))
	{
		UWidgetComponent* HpBar = Enemy->GetHpBarComponent();
		HpBar->bHiddenInGame = true;
	}

	// RagDolling

	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Dissolve

	Dissolve();
}

UAttributeSet* AAuraCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}



void AAuraCharacterBase::OnHitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;

	AAuraAIController* AIController = Cast<AAuraAIController>(GetController());
	if (AIController && AIController->GetBlackboardComponent())
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_bHIT_REACTING, bHitReacting);
	}

	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}






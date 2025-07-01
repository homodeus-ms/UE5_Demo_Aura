
#include "Actor/AuraProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interface/CombatInterface.h"
#include "AuraDefines.h"
#include "AuraLog.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

AAuraProjectile::AAuraProjectile()
{
 	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetCollisionObjectType(ECC_Projectile);
	RootComponent = SphereComponent;

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicateMovement(true);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
	SetLifeSpan(LifeSpanTime);
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(
		LoopingSound, GetRootComponent(), NAME_None, FVector::ZeroVector, EAttachLocation::KeepRelativeOffset, true);
}

void AAuraProjectile::Destroyed()
{
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}

	// For Client
	if (!HasAuthority() && !bClientAlreadyHandledOnHit)
	{
		OnHit();
	}

	Super::Destroyed();
}

void AAuraProjectile::OnHit()
{
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}

	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HitEffect, GetActorLocation());

	if (!HasAuthority())
		bClientAlreadyHandledOnHit = true;
}

void AAuraProjectile::OnTargetEnemyDead(AActor* DeadActor)
{
	if (HasAuthority())
	{
		Destroy();
	}
	else
	{
		OnHit();
	}
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		AActor* Attacker = DamageEffectParams.SourceASC->GetAvatarActor();
		APawn* OtherPawn = Cast<APawn>(OtherActor);

		if (!IsValid(Attacker) || !IsValid(OtherPawn))
		{
			Destroy();
			return;
		}

		bool IsPawn = OtherPawn != nullptr;
		bool bFriend = ICombatInterface::Execute_IsFriend(Attacker, OtherActor);
		bool bSelfAttack = (Attacker == OtherActor);

		if (bSelfAttack || (IsPawn && bFriend))
		{
			return;
		}

		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			DamageEffectParams.TargetASC = TargetASC;

			bool bKnockbackActivate = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
			if (bKnockbackActivate)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;
				FVector Direction = Rotation.Vector();

				FVector KnockbackPos = Direction * DamageEffectParams.KnockbackForce;
				DamageEffectParams.KnockbackPos = KnockbackPos;
			}

			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}

		Destroy();
	}
}

void AAuraProjectile::SetHomingTarget(AActor* Target)
{
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
	{
		FOnDeadDelegate& Delegate = CombatInterface->GetOnDeadDelegate();

		if (!Delegate.IsAlreadyBound(this, &ThisClass::OnTargetEnemyDead))
			Delegate.AddDynamic(this, &ThisClass::OnTargetEnemyDead);
	}

	ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
}




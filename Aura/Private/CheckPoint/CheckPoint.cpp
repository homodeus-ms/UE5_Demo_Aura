// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint/CheckPoint.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Interface/PlayerInterface.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "AuraLog.h"
#include "AuraDefines.h"


ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CheckPointMeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(MeshComponent);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	MoveToComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MoveToComponent"));
	MoveToComponent->SetupAttachment(MeshComponent);

	MeshComponent->SetCustomDepthStencilValue(CUSTOM_DEPTH_WHITE);
	MeshComponent->MarkRenderStateDirty();

}

void ACheckPoint::HighlightActor()
{
	MeshComponent->SetRenderCustomDepth(true);
}

void ACheckPoint::UnHighlightActor()
{
	MeshComponent->SetRenderCustomDepth(false);
}

void ACheckPoint::SetDestinationOnClicked(FVector& OUTDestination)
{
	OUTDestination = MoveToComponent->GetComponentLocation();
}


void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxOverlap);
	
	DynamicMaterial = UMaterialInstanceDynamic::Create(MeshComponent->GetMaterial(0), this);
	MeshComponent->SetMaterial(0, DynamicMaterial);

	MeshComponent->SetCustomDepthStencilValue(CUSTOM_DEPTH_WHITE);
}


void ACheckPoint::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(Aura, Log, TEXT("ACheckPoint::OnBoxOverlap By %s"), HasAuthority() ? TEXT("Server") : TEXT("Client"));


	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OtherActor))
	{
		if (DynamicMaterial)
			OnCheckPointReached(DynamicMaterial);

		if (HasAuthority())
		{
			// Only Server
			FString LevelNamePath = this->GetLevel()->GetOutermost()->GetName();
			const FString& LevelName = FPackageName::GetShortName(LevelNamePath);

			PlayerInterface->SaveCurrentGame(LevelName, PlayerStartTag);
		}
	}
}



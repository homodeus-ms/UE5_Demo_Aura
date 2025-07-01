// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GoldItem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Interface/PlayerInterface.h"

AGoldItem::AGoldItem()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("GoldItemBox"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	TextRenderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderer"));
	TextRenderer->SetupAttachment(RootComponent);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GoldMesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGoldItem::BeginPlay()
{
	Super::BeginPlay();

	Gold = FMath::RandRange(1, 9);
	TextRenderer->SetText(FText::AsNumber(Gold));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxOverlap);
}

void AGoldItem::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OtherActor))
	{
		PlayerInterface->AddGold(Gold);
		Destroy();
	}
}





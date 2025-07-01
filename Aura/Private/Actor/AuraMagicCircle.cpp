// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraMagicCircle.h"
#include "Components/DecalComponent.h"

AAuraMagicCircle::AAuraMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;

	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("MagicCircle"));
	SetRootComponent(MagicCircleDecal);
}

void AAuraMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


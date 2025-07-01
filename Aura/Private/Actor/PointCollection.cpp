// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PointCollection.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* PointZero = CreateDefaultSubobject<USceneComponent>(TEXT("Point0"));
	SetRootComponent(PointZero);
	SpawnPoints.Add(PointZero);

	for (uint8 i = 1; i < MaxSpawnPoint; ++i)
	{
		USceneComponent* Point = CreateDefaultSubobject<USceneComponent>(FName(*FString::Printf(TEXT("Point%d"), i)));
		Point->SetupAttachment(RootComponent);
		SpawnPoints.Add(Point);
	}
}

TArray<USceneComponent*> APointCollection::GetSpawnLocation(const FVector& Center, int32 PointsNum, float RotateYaw)
{
	TArray<USceneComponent*> ResultPoints;
	check(static_cast<uint8>(PointsNum) <= MaxSpawnPoint);

	FVector Pt_ZeroLocation = SpawnPoints[0]->GetComponentLocation();
	FVector Location;
	
	for (int32 i = 0; i < PointsNum; ++i)
	{
		FVector Direction = SpawnPoints[i]->GetComponentLocation() - Pt_ZeroLocation;

		if (i != 0 && !FMath::IsNearlyZero(RotateYaw))
		{
			Direction = Direction.RotateAngleAxis(RotateYaw, FVector::UpVector);
		}

		Location = Pt_ZeroLocation + Direction;

		// Find Z-Value On Surface
		const FVector Start = { Location.X, Location.Y, Location.Z - 500.f };
		const FVector End = { Location.X, Location.Y, Location.Z + 500.f };

		TArray<AActor*> IgnoreActors;
		UAuraAbilitySystemLibrary::GetLiveActorsInRange(this, IgnoreActors, TArray<AActor*>(), Pt_ZeroLocation, 1500.f, EActorSearchType::AllLiveActors);
		
		FHitResult HitPoint;
		FCollisionQueryParams Params;
		Params.bTraceComplex = false;
		Params.AddIgnoredActors(IgnoreActors);

		bool bHit = GetWorld()->LineTraceSingleByProfile(HitPoint, Start, End, FName("BlockAll"), Params);

		Location.Z = HitPoint.ImpactPoint.Z;

		SpawnPoints[i]->SetWorldLocation(Location);
		SpawnPoints[i]->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitPoint.ImpactNormal));

		ResultPoints.Add(SpawnPoints[i]);
	}

	return ResultPoints;
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
}



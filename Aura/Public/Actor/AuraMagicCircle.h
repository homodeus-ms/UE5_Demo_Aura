// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraMagicCircle.generated.h"

UCLASS()
class AURA_API AAuraMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraMagicCircle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UDecalComponent> MagicCircleDecal;
};

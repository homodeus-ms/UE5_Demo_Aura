// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoldItem.generated.h"

UCLASS()
class AURA_API AGoldItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AGoldItem();

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UTextRenderComponent> TextRenderer;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	int32 Gold = 0;

};

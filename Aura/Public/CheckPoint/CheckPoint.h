// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interface/HighlightInterface.h"
#include "CheckPoint.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ACheckPoint : public APlayerStart, public IHighlightInterface
{
	GENERATED_BODY()
	
public:

	// Highlight Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual void SetDestinationOnClicked(FVector& OUTDestination) override;

	ACheckPoint(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCheckPointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);


protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> MoveToComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UMaterialInstanceDynamic> DynamicMaterial = nullptr;
};

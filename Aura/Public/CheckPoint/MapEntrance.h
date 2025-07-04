// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CheckPoint/CheckPoint.h"
#include "MapEntrance.generated.h"


UCLASS()
class AURA_API AMapEntrance : public ACheckPoint
{
	GENERATED_BODY()
	
public:
	AMapEntrance(const FObjectInitializer& ObjectInitializer);

	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnStageCleared();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="NextMap")
	TSoftObjectPtr<UWorld> NextMap;

	UPROPERTY(EditAnywhere, Category="NextMap")
	FName NextMapPlayerStartName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bStageCleared = false;

	UPROPERTY(EditAnywhere)
	bool bHasNextMap = false;

};

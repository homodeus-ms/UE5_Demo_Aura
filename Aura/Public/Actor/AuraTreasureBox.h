// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/OpenableInterface.h"

#include "GameplayTagContainer.h"
#include "AuraTreasureBox.generated.h"

USTRUCT(BlueprintType)
struct FBoxItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ItemTag = FGameplayTag::EmptyTag;

	UPROPERTY(BlueprintReadOnly)
	int32 ItemAmount = 1;
};

UCLASS(Blueprintable, BlueprintType)
class AURA_API AAuraTreasureBox : public AActor, public IOpenableInterface
{
	GENERATED_BODY()
	
public:	

	// Openable Interface
	virtual void OnSelected() override;
	virtual void OnDeselected() override;
	virtual void OpenBox(AActor* Actor) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	AAuraTreasureBox();
	


protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="BoxCollision")
	TObjectPtr<class UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="BoxMesh")
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_BoxITem)
	FBoxItem BoxItem;

private:
	UFUNCTION()
	void OnRep_BoxItem();

	UPROPERTY(EditAnywhere, Category = "ForDebugging")
	bool bCollectItemForDebugging = false;
};

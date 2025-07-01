// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraEffectActor.h"
#include "Interface/ConsumableInterface.h"
#include "GameplayTagContainer.h"
#include "BuyableItem.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ABuyableItem : public AAuraEffectActor, public IConsumableInterface
{
	GENERATED_BODY()

	// Interface
	virtual void Spend(AActor* Target) override;

	FORCEINLINE int32 GetPrice() const { return Price; }
	FORCEINLINE const FGameplayTag& GetItemSpendTag() const { return ItemSpendTag; }

	virtual void OnOverlap(AActor* TargetActor) override;

	virtual void OnEndOverlap(AActor* TargetActor) override;

	
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="ItemInfo")
	int32 Price = 1;

	UPROPERTY(EditDefaultsOnly, Category="ItemInfo")
	FGameplayTag ItemSpendTag;
};

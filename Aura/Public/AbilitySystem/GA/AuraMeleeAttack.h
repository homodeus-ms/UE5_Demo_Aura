// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GA/AuraDamageGiveGA.h"
#include "AuraMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraMeleeAttack : public UAuraDamageGiveGA
{
	GENERATED_BODY()
	
public:
	UAuraMeleeAttack();

	UFUNCTION(BlueprintCallable)
	void Attack(AActor* Target);

};

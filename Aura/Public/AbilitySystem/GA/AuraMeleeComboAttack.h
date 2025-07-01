// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GA/AuraMeleeAttack.h"
#include "AuraMeleeComboAttack.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraMeleeComboAttack : public UAuraMeleeAttack
{
	GENERATED_BODY()
	
public:

	virtual void ExecutePlayMontageAndWait() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnReceiveComboCompletedEventTag(FGameplayEventData Payload);

protected:


	UFUNCTION()
	void ComboInterrupted();
};

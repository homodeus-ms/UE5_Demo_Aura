// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AuraInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputActionTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	const UInputAction* FindInputActionByTag(const FGameplayTag& Tag, bool bLogNotFound = true);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FAuraInputAction> InputActions;
};

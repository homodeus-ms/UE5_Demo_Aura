// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "AuraGABase.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGABase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;

	virtual FString GetDescription(const FString& SkillName, int32 Level) { return FString(); }
	virtual FString GetNextLevelDescription(const FString& SkillName, int32 Level) { return FString(); }
	static FString GetLockedBoxDescription(int32 RequiredLevel);
};

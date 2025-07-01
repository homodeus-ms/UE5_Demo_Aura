// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilityInfo.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag::EmptyTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag TypeTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText SkillName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UGameplayAbility> AbilityClass;

	void Dump() const;
};

UCLASS()
class AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FAuraAbilityInfo FindAbilityByAbilityTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
	FAuraAbilityInfo FindAbilityByStatusTag(const FGameplayTag& StatusTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilityInfo")
	TArray<FAuraAbilityInfo> AbilityInfos;
};

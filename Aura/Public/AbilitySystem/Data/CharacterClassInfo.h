// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScalableFloat.h"
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;
class UGameplayAbility;

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Warrior,
	Magician,
	Ranger,
	Aura,
	Boss1,
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Character Class Deafaults")
	TSubclassOf<UGameplayEffect> PrimaryAttribute;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Deafaults")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Deafaults")
	FScalableFloat XPReward = FScalableFloat();
};

/**
 * 
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FCharacterClassDefaultInfo FindCharacterClassDefaultInfo(ECharacterType Type) const;

	UPROPERTY(EditAnywhere, Category="Character Class Defaults")
	TMap<ECharacterType, FCharacterClassDefaultInfo> CharacterClassInfoMap;

	UPROPERTY(EditAnywhere, Category="Character Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttribute_SetByCaller;

	UPROPERTY(EditAnywhere, Category = "Character Class Defaults")
	TSubclassOf<UGameplayEffect> AuraSecondaryAttribute;

	UPROPERTY(EditAnywhere, Category = "Character Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttribute;

	UPROPERTY(EditAnywhere, Category = "Character Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttribute;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

};

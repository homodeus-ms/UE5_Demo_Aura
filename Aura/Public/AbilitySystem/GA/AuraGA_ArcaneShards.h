// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GA/AuraArcaneSpell.h"
#include "AuraGA_ArcaneShards.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGA_ArcaneShards : public UAuraArcaneSpell
{
	GENERATED_BODY()
	
public:
	UAuraGA_ArcaneShards();

	virtual FString GetDescription(const FString& SkillName, int32 Level = 1) override;
	virtual FString GetNextLevelDescription(const FString& SkillName, int32 Level = 1) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ShardsNum = 1;

	UPROPERTY(EditDefaultsOnly, Category="ArcaneShards")
	int32 MaxShardsNum = 10;
};

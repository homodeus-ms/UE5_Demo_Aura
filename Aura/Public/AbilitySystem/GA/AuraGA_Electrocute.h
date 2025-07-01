// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GA/AuraBeamSpell.h"
#include "AuraGA_Electrocute.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGA_Electrocute : public UAuraBeamSpell
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(const FString& SkillName, int32 Level = 1) override;
	virtual FString GetNextLevelDescription(const FString& SkillName, int32 Level = 1) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Electrocute")
	int32 MaxNumBranches = 5;
};

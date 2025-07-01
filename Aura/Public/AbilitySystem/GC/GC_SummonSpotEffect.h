// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GC_SummonSpotEffect.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UGC_SummonSpotEffect : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SummonEffect")
	TObjectPtr<class UNiagaraSystem> SpotEffect;
};

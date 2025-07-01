// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/NiagaraComponents/AuraNiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"


/**
 * 
 */
UCLASS()
class AURA_API UDebuffNiagaraComponent : public UAuraNiagaraComponent
{
	GENERATED_BODY()
	
public:
	

protected:
	virtual void BeginPlay() override;
};

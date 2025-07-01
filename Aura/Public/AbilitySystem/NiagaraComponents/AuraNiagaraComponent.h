// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameplayTagContainer.h"
#include "AuraNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
	
public:
	UAuraNiagaraComponent();

	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;

	bool IsSkillActive() const;

	UPROPERTY(VisibleAnywhere)
	FGameplayTag NiagaraTag;

protected:

	virtual void BeginPlay() override;

	void OnDebuffTagChangedCallback(const FGameplayTag Tag, int32 NewCount);

	UFUNCTION()
	void OnActorDead(AActor* DeadActor);

	bool bSkillActive = false;

};

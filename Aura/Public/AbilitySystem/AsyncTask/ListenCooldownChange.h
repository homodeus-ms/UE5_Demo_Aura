// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "ListenCooldownChange.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCooldownChangeDelegate, float, Cooltime);

class UAbilitySystemComponent;
struct FGameplayEffectSpec;
/**
 * 
 */
UCLASS(BlueprintType, meta=(ExposedAsyncProxy="AsyncTask"))
class AURA_API UListenCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnCooldownChangeDelegate OnCooldownStart;

	UPROPERTY(BlueprintAssignable)
	FOnCooldownChangeDelegate OnCooldownEnd;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="True"))
	static UListenCooldownChange* ListenToCooldownChange(UAbilitySystemComponent* InASC, const FGameplayTag& InCooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

	UFUNCTION()
	void OnCooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;
};

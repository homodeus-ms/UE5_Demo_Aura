// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TraceTargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCursorTargetDataSignature, const FGameplayAbilityTargetDataHandle, DataHandle);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AURA_API UTraceTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks")
	static UTraceTargetDataUnderMouse* CreateTask(UGameplayAbility* InOwningAbility);

	UPROPERTY(BlueprintAssignable)
	FOnCursorTargetDataSignature OnCursorTargetData;

private:
	virtual void Activate() override;
	void SendOnMouseCursorData();
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);

};

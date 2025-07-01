// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GA/AuraDamageGiveGA.h"
#include "AuraArcaneSpell.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpawnLocationFixedDelegate, const FVector&, Pos);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpawnFinishedDelegate);
/**
 * 
 */

class USceneComponent;

UCLASS()
class AURA_API UAuraArcaneSpell : public UAuraDamageGiveGA
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Should Call this After Receive Broadcast From OnTargetReadyDelegate (Call SetTargetActor() First!)
	UFUNCTION(BlueprintCallable)
	void GetSpawnPoints(int32 InPointNum, float RotationYaw = 0.f);

	UFUNCTION(BlueprintCallable)
	void CleanUp();

	UFUNCTION(BlueprintCallable)
	void SpawnInOrder();

	UPROPERTY(BlueprintAssignable)
	FSpawnLocationFixedDelegate OnLocationFixed;

	UPROPERTY(BlueprintAssignable)
	FSpawnFinishedDelegate OnSpawnFinished;

protected:
	

	UPROPERTY(EditDefaultsOnly, Category = "PointCollection")
	TSubclassOf<class APointCollection> PointCollectActorClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class APointCollection> PointCollectActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<class USceneComponent>> SpawnPoints;

	int32 PointNum = 0;

	FTimerHandle SpawnTimer;

private:
	void DrawDebug(USceneComponent* Next);
};

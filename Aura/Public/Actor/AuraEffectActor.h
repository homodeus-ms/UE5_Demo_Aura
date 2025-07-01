// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EApplyEffectPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	NotApply,
};
UENUM(BlueprintType)
enum class ERemoveEffectPolicy : uint8
{
	RemoveOnApplying,
	RemoveOnEndOverlap,
	NotRemove,
};

USTRUCT(BlueprintType)
struct FAuraGE
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect")
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
	EApplyEffectPolicy ApplyPolicy = EApplyEffectPolicy::NotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
	ERemoveEffectPolicy RemovePolicy = ERemoveEffectPolicy::RemoveOnApplying;
};



UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> AppliedEffectClass);

	UFUNCTION(BlueprintCallable)
	virtual void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	virtual void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Applied")
	bool bDestroyOnEffectApplication = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Applied")
	bool bApplyEffectToEnemy = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Applied")
	FAuraGE InstantEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Applied")
	FAuraGE DurationEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Applied")
	FAuraGE InfiniteEffect;

	TMap<UAbilitySystemComponent*, FActiveGameplayEffectHandle> ActiveEffectMap;
	//TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectsMap;


};

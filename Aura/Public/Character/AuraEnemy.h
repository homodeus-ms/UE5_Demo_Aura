// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interface/EnemyInterface.h"
#include "Interface/HighlightInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UAuraUserWidget;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy(const FObjectInitializer& ObjectInitializer);

	// Interface Functions

	// Highlight Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual void SetDestinationOnClicked(FVector& OUTDestination) override {}

	// Enemy Interface
	
	FORCEINLINE virtual float GetDetactRange() const override { return DetectRange; }
	FORCEINLINE virtual float GetAttackRange() const override { return AttackRange; }
	FORCEINLINE AActor* GetTargetActor_Implementation() const override { return TargetActor; }
	void SetTargetActor_Implementation(AActor* InTargetActor) override { TargetActor = InTargetActor; }
	virtual int32 GetRewardPoint() const override;
	FORCEINLINE virtual class UWidgetComponent* GetHpBarComponent() { return HpBarComponent; }
	virtual bool IsTargetInAttackRange(AActor* InTargetActor) const override;
	
	virtual void AddMinionCount(int32 Count) override;
	FORCEINLINE virtual int32 GetCurrentMinionCount() const override { return MinionCount; }
	virtual void OnMinionDeadCallback(AActor* DestroyedActor) override { AddMinionCount(-1); }
	FORCEINLINE virtual void SetIsSpawnBySummon(bool IsTrue) override { bSpawnBySummon = IsTrue; }
	FORCEINLINE virtual bool IsSpawnBySummon() const override { return bSpawnBySummon; }

	// CharacterBase
	virtual uint8 GetCharacterLevel() override;

	virtual void BeginPlay() override;
	virtual void InitAbilitySystem() override;
	virtual void Die() override;
	virtual void SetLevel(int32 InLevel) override { CharacterLevel = InLevel; }


protected:

	virtual void PossessedBy(AController* NewController) override;
	void InitDefaultAttribute() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attribute|Level")
	uint8 CharacterLevel = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> HpBarComponent;

	UPROPERTY(EditAnywhere, Category="WidgetController")
	TSubclassOf<class UAuraWidgetController> WidgetControllerClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UEnemyWidgetController> EnemyWidgetController;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DisappearDelayTime = 3.f;

	int32 MinionCount = 0;

	// AI
	UPROPERTY(EditAnywhere, Category = "NeedToSetUp|AI")
	TObjectPtr<class UBehaviorTree> BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<class AAuraAIController> AIController;

	UPROPERTY(BlueprintReadWrite, Category="AI|Property")
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(EditDefaultsOnly, Category="NeedToSetUp|AI|Property")
	float DetectRange = 500.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NeedToSetUp|AI|Property")
	float AttackRange = 0.f;

	// Lightning

	bool bShocked = false;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

private:
	void InitHpBarWidget();
	bool bSpawnBySummon = false;
};

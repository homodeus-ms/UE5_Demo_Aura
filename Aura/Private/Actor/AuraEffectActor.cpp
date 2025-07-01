// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraLog.h"
#include "Interface/CombatInterface.h"


AAuraEffectActor::AAuraEffectActor()
{
 	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> AppliedEffectClass)
{
	if (!bApplyEffectToEnemy && Target->ActorHasTag("Enemy"))
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!TargetASC || !AppliedEffectClass)
	{
		UE_LOG(Aura, Warning, TEXT("ApplyEffectToTarget Just Return"));
		return;
	}

	FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(AppliedEffectClass, Cast<ICombatInterface>(Target)->GetCharacterLevel(), ContextHandle);
	const FActiveGameplayEffectHandle ActiveHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	// InfiniteEffect의 경우 나중에 EndOverlap에서 Policy에 따라 제거해줘야함
	// Map에 보관 후 찾아서 지움
	bool bInfinite = SpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bInfinite && InfiniteEffect.RemovePolicy == ERemoveEffectPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectMap.Add(TargetASC, ActiveHandle);
	}
	
	if (!bInfinite)
	{
		Destroy();
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffect.ApplyPolicy == EApplyEffectPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantEffect.EffectClass);
	}
	else if (DurationEffect.ApplyPolicy == EApplyEffectPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationEffect.EffectClass);
	}
	else if (InfiniteEffect.ApplyPolicy == EApplyEffectPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteEffect.EffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffect.ApplyPolicy == EApplyEffectPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantEffect.EffectClass);
	}
	else if (DurationEffect.ApplyPolicy == EApplyEffectPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationEffect.EffectClass);
	}
	else if (InfiniteEffect.ApplyPolicy == EApplyEffectPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteEffect.EffectClass);
	}
	
	if (InfiniteEffect.RemovePolicy == ERemoveEffectPolicy::RemoveOnEndOverlap) 
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (TargetASC && ActiveEffectMap.Contains(TargetASC))
		{
			FActiveGameplayEffectHandle Handle = ActiveEffectMap[TargetASC];
			TargetASC->RemoveActiveGameplayEffect(Handle, 1);
			ActiveEffectMap.Remove(TargetASC);
		}
	}
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AT/TraceTargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AuraLog.h"


UTraceTargetDataUnderMouse* UTraceTargetDataUnderMouse::CreateTask(UGameplayAbility* InOwningAbility)
{
    UTraceTargetDataUnderMouse* NewTask = NewAbilityTask<UTraceTargetDataUnderMouse>(InOwningAbility);
    return NewTask;
}

void UTraceTargetDataUnderMouse::Activate()
{
    if (Ability->GetCurrentActorInfo()->IsLocallyControlled())
    {
        SendOnMouseCursorData();
    }
    else
    {
        // Bind CallBack Func To 'AbilityTargetDataSetDelegate'
        // USE 'CallReplicatedTargetDataDelegatesIfSet()' 
        // if NOT Set yet, Call 'SetWaitingOnRemotePlayerData()'
        FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
        FPredictionKey PredictionKey = GetActivationPredictionKey();

        AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey).AddUObject(
            this, &ThisClass::OnTargetDataReplicatedCallback);
        bool bAlreadySet = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey);
    
        if (!bAlreadySet)
        {
            SetWaitingOnRemotePlayerData();
        }
    }
}

void UTraceTargetDataUnderMouse::SendOnMouseCursorData()
{
    FScopedPredictionWindow PredictionWindow(AbilitySystemComponent.Get());

    // Get HitResult And Send Data To Server

    APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
    FHitResult HitResult;
    PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

    // const FVector HitLocation = HitResult.ImpactPoint;
    // DrawDebugSphere(HitResult.GetActor()->GetWorld(), HitLocation, 20.f, 16, FColor::Red, false, 5.f);

    FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
    FPredictionKey PredictionKey = GetActivationPredictionKey();
    FGameplayAbilityTargetDataHandle DataHandle;

    // In Engine code, It Becomes 'SharedPtr' When DataHandle.Add().
    FGameplayAbilityTargetData_SingleTargetHit* SingleTargetData = new FGameplayAbilityTargetData_SingleTargetHit();
    SingleTargetData->HitResult = HitResult;
    DataHandle.Add(SingleTargetData);

    AbilitySystemComponent.Get()->ServerSetReplicatedTargetData(
        SpecHandle, PredictionKey, DataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

    if (ShouldBroadcastAbilityTaskDelegates())
    {
        OnCursorTargetData.Broadcast(DataHandle);
    }
}

void UTraceTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
    // Notify to Server Can Consume Data hold
    // Broadcast TargetData
    AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
    
    if (ShouldBroadcastAbilityTaskDelegates())
    {
        OnCursorTargetData.Broadcast(DataHandle);
    }
}

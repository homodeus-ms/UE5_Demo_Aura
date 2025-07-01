// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTask/ListenCooldownChange.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "AuraLog.h"

UListenCooldownChange* UListenCooldownChange::ListenToCooldownChange(UAbilitySystemComponent* InASC, const FGameplayTag& InCooldownTag)
{
    if (!IsValid(InASC) || !InCooldownTag.IsValid())
        return nullptr;

    UListenCooldownChange* ListenTask = NewObject<UListenCooldownChange>();
    ListenTask->ASC = InASC;
    ListenTask->CooldownTag = InCooldownTag;

    InASC->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
        ListenTask, &UListenCooldownChange::OnCooldownTagChanged);

    return ListenTask;
}

void UListenCooldownChange::EndTask()
{
    if (!IsValid(ASC))
        return;

    ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

    SetReadyToDestroy();
    MarkAsGarbage();
}

void UListenCooldownChange::OnCooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
    if (NewCount == 0)
    {
        OnCooldownEnd.Broadcast(0.f);
    }
    else
    {
        FGameplayEffectQuery EffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
        TArray<float> TimeRemaings = ASC->GetActiveEffectsTimeRemaining(EffectQuery);

        float RemainTime = 0.f;

        for (float Time : TimeRemaings)
        {
            if (RemainTime < Time)
            {
                RemainTime = Time;
            }
        }

        OnCooldownStart.Broadcast(RemainTime);
    }
}


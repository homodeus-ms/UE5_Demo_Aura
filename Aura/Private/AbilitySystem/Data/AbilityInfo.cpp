// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"
#include "AuraLog.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityByAbilityTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
    for (const FAuraAbilityInfo& Info : AbilityInfos)
    {
        if (Info.AbilityTag.MatchesTagExact(AbilityTag))
        {
            return Info;
        }
    }

    if (bLogNotFound)
    {
        UE_LOG(Aura, Warning, TEXT("No Ability in AbilityInfos, %s"), __FUNCTION__);
    }

    return FAuraAbilityInfo();
}

FAuraAbilityInfo UAbilityInfo::FindAbilityByStatusTag(const FGameplayTag& StatusTag, bool bLogNotFound) const
{
    for (const FAuraAbilityInfo& Info : AbilityInfos)
    {
        if (Info.StatusTag.MatchesTagExact(StatusTag))
        {
            return Info;
        }
    }

    if (bLogNotFound)
    {
        UE_LOG(Aura, Warning, TEXT("No StatusTag in AbilityInfos, %s"), __FUNCTION__);
    }

    return FAuraAbilityInfo();
}

void FAuraAbilityInfo::Dump() const
{
    UE_LOG(Aura, Log, TEXT("----- FAuraAbilityInfo Dump -----"));
    UE_LOG(Aura, Log, TEXT("AbilityTag: %s"), *AbilityTag.ToString());
    UE_LOG(Aura, Log, TEXT("InputTag: %s"), *InputTag.ToString());
    UE_LOG(Aura, Log, TEXT("CooldownTag: %s"), *CooldownTag.ToString());
    UE_LOG(Aura, Log, TEXT("StatusTag: %s"), *StatusTag.ToString());
    UE_LOG(Aura, Log, TEXT("TypeTag: %s"), *TypeTag.ToString());
    UE_LOG(Aura, Log, TEXT("SkillName: %s"), *SkillName.ToString());
    UE_LOG(Aura, Log, TEXT("LevelRequirement: %d"), LevelRequirement);

    // AbilityClass는 nullptr일 수 있으므로 체크
    if (AbilityClass)
    {
        UE_LOG(Aura, Log, TEXT("AbilityClass: %s"), *AbilityClass->GetName());
    }
    else
    {
        UE_LOG(Aura, Log, TEXT("AbilityClass: nullptr"));
    }
}

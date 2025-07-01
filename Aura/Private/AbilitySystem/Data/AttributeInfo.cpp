// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& AttributeTag, bool bLogIfNotFound) const
{
    for (const FAuraAttributeInfo& Info : AttributeInfoArray)
    {
        if (Info.AttributeTag.MatchesTagExact(AttributeTag))
        {
            return Info;
        }
    }

    if (bLogIfNotFound)
    {
        UE_LOG(LogTemp, Error, TEXT("Can't Find Matching AuraAttributeInfo with Tag"));
    }

    return FAuraAttributeInfo();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"
#include "InputAction.h"
#include "AuraLog.h"

const UInputAction* UAuraInputConfig::FindInputActionByTag(const FGameplayTag& Tag, bool bLogNotFound)
{
    for (const FAuraInputAction& AuraInputAction : InputActions)
    {
        if (AuraInputAction.InputActionTag.MatchesTagExact(Tag))
        {
            return AuraInputAction.InputAction;
        }
    }

    if (bLogNotFound)
    {
        UE_LOG(Aura, Warning, TEXT("Can't find InputAction Matches with Tag"));
    }

    return nullptr;
}

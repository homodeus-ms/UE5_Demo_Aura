// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/APIData.h"

FString UAPIData::GetAPIEndpoint(const FGameplayTag& ResourceTag)
{
    const FString& ResourceName = ResourceNames.FindChecked(ResourceTag);
    return InvokeURL + "/" + Stage + "/" + ResourceName;
}

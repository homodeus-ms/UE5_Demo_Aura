// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "APIData.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API UAPIData : public UDataAsset
{
	GENERATED_BODY()
	
public:

	FString GetAPIEndpoint(const FGameplayTag& ResourceTag);

protected:
	UPROPERTY(EditDefaultsOnly)
	FString APIName;

	UPROPERTY(EditDefaultsOnly)
	FString InvokeURL;

	UPROPERTY(EditDefaultsOnly)
	FString Stage;

	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FString> ResourceNames;
};

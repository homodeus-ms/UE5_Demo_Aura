// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AuraGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	virtual void Init() override;

	void SetInfo(const FName& InPlayerStartTag, const FString& InSlotName, int32 InSlotIndex);

	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()
	FString LoadSlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;

	FORCEINLINE class UActorFactoryMap* GetFactoryMap() const { return FactoryMap; }

protected:

	UPROPERTY()
	TObjectPtr<class UActorFactoryMap> FactoryMap;
};

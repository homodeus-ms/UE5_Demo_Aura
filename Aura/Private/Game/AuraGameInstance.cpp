// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AuraGameInstance.h"
#include "ActorFactoryMap.h"

void UAuraGameInstance::Init()
{
	Super::Init();

	FactoryMap = NewObject<UActorFactoryMap>(this);
	FactoryMap->RegisterMap();
}

void UAuraGameInstance::SetInfo(const FName& InPlayerStartTag, const FString& InSlotName, int32 InSlotIndex)
{
	PlayerStartTag = InPlayerStartTag;
	LoadSlotName = InSlotName;
	SlotIndex = InSlotIndex;
}

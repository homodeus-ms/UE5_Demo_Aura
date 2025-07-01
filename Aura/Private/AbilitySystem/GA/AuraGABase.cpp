// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA/AuraGABase.h"

FString UAuraGABase::GetLockedBoxDescription(int32 RequiredLevel)
{
	return FString::Printf(TEXT("<Middle>Level</> <Level>%d</>에\n스킬을 볼 수 있습니다"), RequiredLevel);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AuraLog.h"

const FAuraLevelUpInfo& ULevelUpInfo::GetLevelUpInfo(int32 Level) const
{
	check(Level >= 1 && Level <= LevelUpInfos.Num());

	return LevelUpInfos[Level - 1];
}

int32 ULevelUpInfo::GetCurrentLevelMaxXP(int32 Level) const
{
	return GetLevelUpInfo(Level).LevelUpRequirement;
}

int32 ULevelUpInfo::GetMaxXP() const
{
	return GetCurrentLevelMaxXP(LevelUpInfos.Num());
}

int32 ULevelUpInfo::FindLevelForXP(int32 XP)
{
	if (XP >= GetMaxXP())
		return LevelUpInfos.Num();

	int32 Level = 0;
	int32 CurrentMaxXP = 0;
	do
	{
		++Level;
		CurrentMaxXP = GetCurrentLevelMaxXP(Level);
	} while (XP > CurrentMaxXP);

	return Level;
}









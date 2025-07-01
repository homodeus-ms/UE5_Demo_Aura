// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA/AuraGA_ArcaneShards.h"

UAuraGA_ArcaneShards::UAuraGA_ArcaneShards()
{
	int32 Num = FMath::Max(GetAbilityLevel() - 2, 1);
	ShardsNum = FMath::Min(Num, MaxShardsNum);
}

FString UAuraGA_ArcaneShards::GetDescription(const FString& SkillName, int32 Level)
{
	const float Damage = GetDamageByLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooltime = GetCooltime(Level);


	return FString::Printf(TEXT(
		"<Title>%s</>\n"
		"<Middle>(Lv. </><Level>%d</><Middle>)</>\n\n"
		"데미지 - <Damage>%0.1f</>\n"
		"마나소비 - <Cost>%0.1f</>\n"
		"쿨타임 - <Cooltime>%0.1f</>\n\n"
		"<Level>%d</>개의 파편을 스폰. 파편당 가까운 타겟에게 최대 <Damage>%0.1f</>의 범위데미지를 줍니다.\n\n"
		
	),

		*SkillName, Level, Damage, ManaCost, Cooltime, ShardsNum, Damage);
}

FString UAuraGA_ArcaneShards::GetNextLevelDescription(const FString& SkillName, int32 Level)
{
	++Level;
	const float Damage = GetDamageByLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooltime = GetCooltime(Level);

	return FString::Printf(TEXT(
		"<Title>%s</>\n"
		"<Middle>(Lv. </><Level>%d</><Middle>)</>\n\n"
		"데미지 - <Damage>%0.1f</>\n"
		"마나소비 - <Cost>%0.1f</>\n"
		"쿨타임 - <Cooltime>%0.1f</>\n\n"
		"<Level>%d</>개의 파편을 스폰. 파편당 가까운 타겟에게 최대 <Damage>%0.1f</>의 범위데미지를 줍니다.\n\n"

	),

		*SkillName, Level, Damage, ManaCost, Cooltime, ShardsNum + 1, Damage);
}
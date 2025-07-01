// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA/AuraGA_Electrocute.h"

FString UAuraGA_Electrocute::GetDescription(const FString& SkillName, int32 Level)
{
    const float Damage = GetDamageByLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooltime = GetCooltime(Level);
	const int32 NumBranches = FMath::Min(Level, MaxNumBranches);
	

	return FString::Printf(TEXT(
		"<Title>%s</>\n"
		"<Middle>(Lv. </><Level>%d</><Middle>)</>\n\n"
		"데미지 - <Damage>%0.1f</> (<Time>%0.1f</>초당)\n"
		"마나소비 - <Cost>%0.1f</>\n"
		"쿨타임 - <Cooltime>%0.1f</>\n\n"
		"타겟에게 <Time>%0.1f</>초당 <Damage>%0.1f</>의 데미지, <Level>%d</>명의 근처 타겟에게도 추가 피해를 줍니다.\n\n"
		"<DebuffTitle>기절 디버프</>\n"
		"디버프 발생 확률 : <Chance>%0.f%%</>\n"
		"기절 디버프가 발생하면 일정 시간동안 적은 아무것도 할 수 없습니다.\n"
	),

		*SkillName, Level, Damage, DamageDeltaTime, ManaCost, Cooltime, DamageDeltaTime, Damage, NumBranches, DebuffChance);
}

FString UAuraGA_Electrocute::GetNextLevelDescription(const FString& SkillName, int32 Level)
{
	Level++;
	const float Damage = GetDamageByLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooltime = GetCooltime(Level);
	const int32 NumBranches = FMath::Min(Level, MaxNumBranches);

	return FString::Printf(TEXT(
		"<Title>%s</>\n"
		"<Middle>(Lv. </><Level>%d</><Middle>)</>\n\n"
		"데미지 - <Damage>%0.1f</> (<Time>%0.1f</>초당)\n"
		"마나소비 - <Cost>%0.1f</>\n"
		"쿨타임 - <Cooltime>%0.1f</>\n\n"
		"타겟에게 <Time>%0.1f</>초당 <Damage>%0.1f</>의 데미지, <Level>%d</>명의 근처 타겟에게도 추가 피해를 줍니다.\n"
		"<DebuffTitle>기절 디버프</>\n"
		"디버프 발생 확률 : <Chance>%0.f%%</>\n"
		"기절 디버프가 발생하면 일정 시간동안 적은 아무것도 할 수 없습니다.\n"
	),

		*SkillName, Level, Damage, DamageDeltaTime, ManaCost, Cooltime, DamageDeltaTime, Damage, NumBranches, DebuffChance);
}

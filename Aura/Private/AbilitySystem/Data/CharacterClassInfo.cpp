// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::FindCharacterClassDefaultInfo(ECharacterType Type) const
{
	return CharacterClassInfoMap.FindChecked(Type);
}

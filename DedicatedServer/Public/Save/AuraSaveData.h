#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AuraSaveData.generated.h"

USTRUCT()
struct FUserInfoSaveData
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "userName"))
	FString UserName{};

	UPROPERTY()
	bool bNewPlayer = true;
};

USTRUCT()
struct FPlayerInfoSaveData
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "playerLevel"))
	int32 PlayerLevel = 1;

	UPROPERTY(meta = (JsonProperty = "xp"))
	int32 Xp = 0;

	UPROPERTY(meta = (JsonProperty = "spellPoint"))
	int32 SpellPoint = 1;

	UPROPERTY(meta = (JsonProperty = "attributePoint"))
	int32 AttributePoint = 1;

	UPROPERTY(meta = (JsonProperty = "mapName"))
	FString MapName = TEXT("Lobby");

	UPROPERTY(meta = (JsonProperty = "playerStart"))
	FString PlayerStart{};

	void Dump() const;
};

USTRUCT()
struct FAttributesSaveData
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "strength"))
	float Strength = 0.f;

	UPROPERTY(meta = (JsonProperty = "intelligence"))
	float Intelligence = 0.f;

	UPROPERTY(meta = (JsonProperty = "resilience"))
	float Resilience = 0.f;

	UPROPERTY(meta = (JsonProperty = "vigor"))
	float Vigor = 0.f;

	UPROPERTY(meta = (JsonProperty = "currentHp"))
	float CurrentHP = 0.f;

	UPROPERTY(meta = (JsonProperty = "currentMp"))
	float CurrentMP = 0.f;

	void Dump() const;
};

USTRUCT()
struct FInventorySaveData
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "gold"))
	int32 Gold = 0;

	UPROPERTY(meta = (JsonProperty = "inventoryItemAmounts"))
	TMap<FString, int32> InventoryItemAmounts;

	UPROPERTY(meta = (JsonProperty = "collections"))
	TArray<FString> Collections;

	void Dump() const;
};

USTRUCT()
struct FSkillSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FString AbilityClassPath;

	UPROPERTY()
	FString AbilityTag{};

	UPROPERTY()
	FString AbilitySlot{};

	UPROPERTY()
	FString AbilityStatus{};

	UPROPERTY()
	FString AbilityType{};

	UPROPERTY()
	int32 AbilityLevel = 1;

	void Dump() const;
};

inline bool operator==(const FSkillSaveData& Left, const FSkillSaveData& Right)
{
	return Left.AbilityTag.Equals(Right.AbilityTag);
}



UCLASS()
class DEDICATEDSERVER_API UAuraSaveData : public UObject
{
    GENERATED_BODY()

public:

	UPROPERTY(meta = (JsonProperty = "playerData"))
	FPlayerInfoSaveData PlayerData{};

	UPROPERTY(meta = (JsonProperty = "attributesData"))
	FAttributesSaveData AttributesData{};

	UPROPERTY(meta = (JsonProperty = "inventoryData"))
	FInventorySaveData InventoryData{};

	UPROPERTY(meta = (JsonProperty = "savedSkills"))
	TArray<FSkillSaveData> SavedSkills{};
};
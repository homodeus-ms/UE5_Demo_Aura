#include "Save/AuraSaveData.h"
#include "Common/DedicatedServer.h"

void FPlayerInfoSaveData::Dump() const
{
	UE_LOG(LogAuraDS, Log, TEXT("----- FPlayerInfoSaveData Dump -----"));
	UE_LOG(LogAuraDS, Log, TEXT("PlayerLevel: %d"), PlayerLevel);
	UE_LOG(LogAuraDS, Log, TEXT("Xp: %d"), Xp); 
	UE_LOG(LogAuraDS, Log, TEXT("SpellPoint: %d"), SpellPoint); 
	UE_LOG(LogAuraDS, Log, TEXT("AttributePoint: %d"), AttributePoint); 
	UE_LOG(LogAuraDS, Log, TEXT("MapName: %s"), *MapName);
	UE_LOG(LogAuraDS, Log, TEXT("PlayerStart: %s"), *PlayerStart);
}

void FAttributesSaveData::Dump() const
{
	UE_LOG(LogAuraDS, Log, TEXT("----- FAttributesSaveData Dump -----"));
	UE_LOG(LogAuraDS, Log, TEXT("Strength: %.2f"), Strength);
	UE_LOG(LogAuraDS, Log, TEXT("Intelligence: %.2f"), Intelligence);
	UE_LOG(LogAuraDS, Log, TEXT("Resilience: %.2f"), Resilience);
	UE_LOG(LogAuraDS, Log, TEXT("Vigor: %.2f"), Vigor);
	UE_LOG(LogAuraDS, Log, TEXT("CurrentHP: %.2f"), CurrentHP);
	UE_LOG(LogAuraDS, Log, TEXT("CurrentMP: %.2f"), CurrentMP);
}

void FInventorySaveData::Dump() const
{
	UE_LOG(LogAuraDS, Log, TEXT("----- FInventorySaveData Dump -----"));
	UE_LOG(LogAuraDS, Log, TEXT("Gold: %d"), Gold);

	UE_LOG(LogAuraDS, Log, TEXT("InventoryItemAmounts:"));
	for (const auto& Pair : InventoryItemAmounts)
	{
		UE_LOG(LogAuraDS, Log, TEXT("  %s: %d"), *Pair.Key, Pair.Value);
	}

	UE_LOG(LogAuraDS, Log, TEXT("Collections:"));
	for (const FString& Item : Collections)
	{
		UE_LOG(LogAuraDS, Log, TEXT("  %s"), *Item);
	}
}

void FSkillSaveData::Dump() const
{
	UE_LOG(LogAuraDS, Log, TEXT("----- FSkillSaveData Dump -----"));
	UE_LOG(LogAuraDS, Log, TEXT("AbilityClassPath: %s"), *AbilityClassPath);
	UE_LOG(LogAuraDS, Log, TEXT("AbilityTag: %s"), *AbilityTag);
	UE_LOG(LogAuraDS, Log, TEXT("AbilitySlot: %s"), *AbilitySlot);
	UE_LOG(LogAuraDS, Log, TEXT("AbilityStatus: %s"), *AbilityStatus);
	UE_LOG(LogAuraDS, Log, TEXT("AbilityType: %s"), *AbilityType);
	UE_LOG(LogAuraDS, Log, TEXT("AbilityLevel: %d"), AbilityLevel);
}
#pragma once

#include "AuraLog.h"
#include "GameplayTagContainer.h"
#include "AuraTypes.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct FInventoryItemEntry
{
	GENERATED_BODY()

	FInventoryItemEntry() {}

	FInventoryItemEntry(const FGameplayTag& InItemTag, int32 Count) : ItemTag(InItemTag), ItemCount(Count) {}

	bool operator==(const FInventoryItemEntry & Other) const
	{
		return ItemTag.MatchesTagExact(Other.ItemTag);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ItemTag{};
	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemCount = 0;
};

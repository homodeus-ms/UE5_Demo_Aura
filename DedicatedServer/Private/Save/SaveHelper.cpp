#include "Save/SaveHelper.h"
#include "DedicatedServer/Public/Common/DedicatedServer.h"

void FSaveHelper::ConvertTagMapToStringMap(const TMap<FGameplayTag, int32>& OriginalMap, TMap<FString, int32>& OUTDataMap)
{
	for (const auto& Pair : OriginalMap)
	{
		OUTDataMap.Add(Pair.Key.ToString(), Pair.Value);
	}
}

void FSaveHelper::ConvertTagArrayToStringArray(const TArray<FGameplayTag>& OriginalArr, TArray<FString>& OUTDataArr)
{
	for (const auto& Entry : OriginalArr)
	{
		OUTDataArr.Add(Entry.ToString());
	}
}


FString FSaveHelper::ConvertTagToString(const FGameplayTag& Tag)
{
	return Tag.ToString();
}

void FSaveHelper::ConvertStringMapToTagMap(const TMap<FString, int32>& OriginalMap, TMap<FGameplayTag, int32>& OUTDataMap)
{
	for (const auto& Pair : OriginalMap)
	{
		const FGameplayTag& GameplayTag = ConvertStringToTag(Pair.Key);
		if (!GameplayTag.IsValid())
		{
			UE_LOG(LogAuraDS, Warning, TEXT("Convert Map, String->GameplayTag Is NOT Valid"));
		}
		OUTDataMap.Add(GameplayTag, Pair.Value);
	}
}

void FSaveHelper::ConvertStringArrayToTagArray(const TArray<FString>& OriginalArr, TArray<FGameplayTag>& OUTDataArr)
{
	for (const FString& Str : OriginalArr)
	{
		const FGameplayTag& GameplayTag = ConvertStringToTag(Str);
		if (!GameplayTag.IsValid())
		{
			UE_LOG(LogAuraDS, Warning, TEXT("Convert Array, String->GameplayTag Is NOT Valid"));
		}
		OUTDataArr.Add(GameplayTag);
	}
}


FGameplayTag FSaveHelper::ConvertStringToTag(const FString& Str)
{
	if (Str.IsEmpty() || Str == TEXT("None"))
	{
		return FGameplayTag();
	}
	return FGameplayTag::RequestGameplayTag(FName(*Str));
}

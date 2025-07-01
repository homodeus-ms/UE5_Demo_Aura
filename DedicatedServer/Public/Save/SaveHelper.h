#pragma once

#include "GameplayTagContainer.h"

class DEDICATEDSERVER_API FSaveHelper 
{
public:
	static void ConvertTagMapToStringMap(const TMap<FGameplayTag, int32>& OriginalMap, TMap<FString, int32>& OUTDataMap);
	static void ConvertTagArrayToStringArray(const TArray<FGameplayTag>& OriginalArr, TArray<FString>& OUTDataArr);
	
	static FString ConvertTagToString(const FGameplayTag& Tag);

	static void ConvertStringMapToTagMap(const TMap<FString, int32>& OriginalMap, TMap<FGameplayTag, int32>& OUTDataMap);
	static void ConvertStringArrayToTagArray(const TArray<FString>& OriginalArr, TArray<FGameplayTag>& OUTDataArr);
	
	static FGameplayTag ConvertStringToTag(const FString& Str);
};
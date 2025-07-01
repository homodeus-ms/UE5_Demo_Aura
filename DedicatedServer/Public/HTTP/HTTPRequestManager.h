 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HTTPRequestManager.generated.h"

class UAPIData;
class UDS_LocalPlayerSubsystem;
class UDS_GameInstanceSubsystem;
class UAuraSaveData;

#define PLAYER_INFO_TABLE TEXT("PlayerInfo")
#define ATTRIBUTES_TABLE TEXT("Attributes")
#define INVENTORY_TABLE TEXT("Inventory")
#define SKILLS_TABLE TEXT("Skills")

struct FAPIRequestSendData
{
	FAPIRequestSendData(const FGameplayTag& Tag, const FString& InVerb) : APITag(Tag), Verb(InVerb) {}

	FGameplayTag APITag{};
	FString Verb{};
	TPair<FString, FString> AdditionalHeader{};
	TMap<FString, FString> Params{};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStatusMessageDelegate, const FString&, Message, bool, bShouldResetWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAPIRequestSucceeded);

/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API UHTTPRequestManager : public UObject
{
	GENERATED_BODY()
	
public:
	void SendHttpRequest(TSharedRef<IHttpRequest>& Request, const FAPIRequestSendData& Data);

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAPIData> APIData;

	bool ContainsErrors(const TSharedPtr<FJsonObject>& JsonObject);

	FString SerializeJsonContent(const TMap<FString, FString>& Params);
	FString SerializeJsonContent(const UStruct* StructType, const void* StructPtr);
	TSharedPtr<FJsonObject> StructToJsonObject(const UStruct* StructType, const void* StructPtr);
	TSharedPtr<FJsonValueObject> GetJsonValueObject(const FString& TableName, const TSharedPtr<FJsonObject>& Data);

	UDS_LocalPlayerSubsystem* GetDSLocalPlayerSubsystem() const;
	UDS_GameInstanceSubsystem* GetDSGameInstanceSubsystem() const;

};

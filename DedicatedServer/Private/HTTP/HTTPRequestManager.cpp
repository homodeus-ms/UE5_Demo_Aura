// Fill out your copyright notice in the Description page of Project Settings.


#include "HTTP/HTTPRequestManager.h"
#include "Data/APIData.h"
#include "HttpModule.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Common/DedicatedServer.h"
#include "Player/DS_LocalPlayerSubsystem.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "Game/DS_GameModeBase.h"
#include "JsonObjectConverter.h"

void UHTTPRequestManager::SendHttpRequest(TSharedRef<IHttpRequest>& Request, const FAPIRequestSendData& Data)
{
    const FString& EndpointURL = APIData->GetAPIEndpoint(Data.APITag);

    Request->SetURL(EndpointURL);
    Request->SetVerb(Data.Verb);
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	if (!Data.AdditionalHeader.Key.IsEmpty())
	{
		Request->SetHeader(Data.AdditionalHeader.Key, Data.AdditionalHeader.Value);
	}

	if (!Data.Params.IsEmpty())
	{
		const FString& Content = SerializeJsonContent(Data.Params);
		Request->SetContentAsString(Content);
	}
	
	Request->ProcessRequest();
}

bool UHTTPRequestManager::ContainsErrors(const TSharedPtr<FJsonObject>& JsonObject)
{
	bool bHasErrorType = JsonObject->HasField(TEXT("errorType"));
	bool bHasErrorMessage = JsonObject->HasField(TEXT("errorMessage"));
	if (bHasErrorType || bHasErrorMessage)
	{
		if (bHasErrorType)
		{
			const FString& ErrorType = JsonObject->GetStringField(TEXT("errorType"));
			UE_LOG(LogAuraDS, Error, TEXT("ErrorType: %s"), *ErrorType);
		}
		if (bHasErrorMessage)
		{
			const FString& ErrorMessage = JsonObject->GetStringField(TEXT("errorMessage"));
			UE_LOG(LogAuraDS, Error, TEXT("ErrorMessage: %s"), *ErrorMessage);
		}
		return true;
	}

	if (JsonObject->HasField(TEXT("$fault")))
	{
		const FString ErrorType = JsonObject->HasField(TEXT("name")) ? JsonObject->GetStringField(TEXT("name")) : "Unknown Error";
		UE_LOG(LogAuraDS, Error, TEXT("ErrorMessage: %s"), *ErrorType);
		return true;
	}

    return false;
}

FString UHTTPRequestManager::SerializeJsonContent(const TMap<FString, FString>& Params)
{
	TSharedPtr<FJsonObject> JsonObjectPtr = MakeShareable(new FJsonObject);

	for (const auto& Pair : Params)
	{
		JsonObjectPtr->SetStringField(Pair.Key, Pair.Value);
	}

	FString Content;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObjectPtr.ToSharedRef(), JsonWriter);

	return Content;
}

FString UHTTPRequestManager::SerializeJsonContent(const UStruct* StructType, const void* StructPtr)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	FJsonObjectConverter::UStructToJsonObject(StructType, StructPtr, JsonObject.ToSharedRef());
	FString ResultString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&ResultString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	return ResultString;
}

TSharedPtr<FJsonObject> UHTTPRequestManager::StructToJsonObject(const UStruct* StructType, const void* StructPtr)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	FJsonObjectConverter::UStructToJsonObject(StructType, StructPtr, JsonObject.ToSharedRef());
	return JsonObject;
}

TSharedPtr<FJsonValueObject> UHTTPRequestManager::GetJsonValueObject(const FString& TableName, const TSharedPtr<FJsonObject>& Data)
{
	TSharedPtr<FJsonObject> Entry = MakeShared<FJsonObject>();
	Entry->SetStringField(TEXT("tableName"), TableName);
	Entry->SetObjectField(TEXT("data"), Data);
	return MakeShared<FJsonValueObject>(Entry);
}

UDS_LocalPlayerSubsystem* UHTTPRequestManager::GetDSLocalPlayerSubsystem() const
{
	APlayerController* PlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(PlayerController))
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);
		if (IsValid(LocalPlayer))
		{
			return LocalPlayer->GetSubsystem<UDS_LocalPlayerSubsystem>();
		}
	}
	return nullptr;
}

UDS_GameInstanceSubsystem* UHTTPRequestManager::GetDSGameInstanceSubsystem() const
{
	if (UWorld* World = GetWorld())
	{
		if (ADS_GameModeBase* GameMode = Cast<ADS_GameModeBase>(World->GetAuthGameMode()))
		{
			return GameMode->GetDSGameInstanceSubsystem();
		}
	}
	
	return nullptr;
}



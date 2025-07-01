// Fill out your copyright notice in the Description page of Project Settings.

#include "HTTP/SaveManager.h"
#include "Common/DedicatedServer.h"
#include "HttpModule.h"
#include "DedicatedServer/DedicatedServerTags.h"
#include "JsonObjectConverter.h"
#include "Player/DS_PlayerState.h"
#include "Save/AuraSaveData.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "Player/DS_LocalPlayerSubsystem.h"
#include "Data/APIData.h"

USaveManager::USaveManager()
{
	
}

void USaveManager::SaveGame(ADS_PlayerState* DSPlayerState)
{
	UAuraSaveData* SaveData = DSPlayerState->GetSaveData();
	const FString& Sub = DSPlayerState->GetSub();

	if (!IsValid(SaveData))
	{
		UE_LOG(LogAuraDS, Warning, TEXT("SaveData Is NOT Vaild. %s"), *FString(__FUNCTION__));
		return;
	}

	DSPlayerState->SavePlayersLastMapInfoToGISubsystem(SaveData->PlayerData.MapName, SaveData->PlayerData.PlayerStart);
	
	UE_LOG(LogAuraDS, Log, TEXT("----- SaveManager, SaveGame -----"));
	
	const TArray<FSkillSaveData>& CheckArray = SaveData->SavedSkills;
	for (const FSkillSaveData& Skill : CheckArray)
	{
		UE_LOG(LogAuraDS, Log, TEXT("Skill: %s, Level: %d"), *Skill.AbilityTag, Skill.AbilityLevel);
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::SaveGame_Response);

	check(APIData);

	FAPIRequestSendData SendData(DedicatedServerTags::SaveDataAPI::SaveData, TEXT("POST"));
	
	TSharedPtr<FJsonObject> Root = MakeShared<FJsonObject>();
	Root->SetStringField(TEXT("sub"), Sub);

	TArray<TSharedPtr<FJsonValue>> InfoArray;

	TSharedPtr<FJsonObject> PlayerDataObj = StructToJsonObject(FPlayerInfoSaveData::StaticStruct(), &SaveData->PlayerData);
	TSharedPtr<FJsonObject> StatsDataObj = StructToJsonObject(FAttributesSaveData::StaticStruct(), &SaveData->AttributesData);
	TSharedPtr<FJsonObject> InventoryDataObj = StructToJsonObject(FInventorySaveData::StaticStruct(), &SaveData->InventoryData);
	
	InfoArray.Add(GetJsonValueObject(PLAYER_INFO_TABLE, PlayerDataObj));
	InfoArray.Add(GetJsonValueObject(ATTRIBUTES_TABLE, StatsDataObj));
	InfoArray.Add(GetJsonValueObject(INVENTORY_TABLE, InventoryDataObj));

	TArray<TSharedPtr<FJsonValue>> SkillsArr;
	for (const FSkillSaveData& SkillsData : SaveData->SavedSkills)
	{
		UE_LOG(LogAuraDS, Log, TEXT("Skill Data Check Before Making Save Data"));
		SkillsData.Dump();
		TSharedPtr<FJsonObject> SkillObj = StructToJsonObject(FSkillSaveData::StaticStruct(), &SkillsData);
		
		// TEMP
		FString TempCheckStr;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&TempCheckStr);
		FJsonSerializer::Serialize(SkillObj.ToSharedRef(), Writer);

		SkillsArr.Add(MakeShared<FJsonValueObject>(SkillObj));
	}

	TSharedPtr<FJsonObject> SkillsObj = MakeShared<FJsonObject>();
	SkillsObj->SetStringField(TEXT("tableName"), SKILLS_TABLE);
	SkillsObj->SetArrayField(TEXT("data"), SkillsArr);
	InfoArray.Add(MakeShared<FJsonValueObject>(SkillsObj));
	
	Root->SetArrayField(TEXT("infos"), InfoArray);

	FString SaveDataStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&SaveDataStr);
	FJsonSerializer::Serialize(Root.ToSharedRef(), Writer);

	Request->SetContentAsString(SaveDataStr);

	SendHttpRequest(Request, SendData);
}

void USaveManager::LoadGame(const FString& Sub, FOnRetrieveInfosDelegate Callback)
{
	UE_LOG(LogAuraDS, Log, TEXT("USaveManager::LoadGame"));

	RetrieveCallbacksMap.Add(Sub, Callback);

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::LoadGame_Response);

	check(APIData);

	FAPIRequestSendData SendData(DedicatedServerTags::SaveDataAPI::LoadData, TEXT("POST"));
	SendData.Params.Add(TEXT("sub"), Sub);

	SendHttpRequest(Request, SendData);
}

FOnRetrieveInfosDelegate& USaveManager::GetRetrieveInfosDelegate()
{
	return OnRetrieveInfos;
}

void USaveManager::SetNotANewPlayer(ADS_PlayerState* DSPlayerState)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::SetNotANewPlayer_Response);

	check(APIData);

	FAPIRequestSendData SendData(DedicatedServerTags::SaveDataAPI::SetNotANewPlayer, TEXT("POST"));
	SendData.Params.Add(TEXT("sub"), DSPlayerState->GetSub());

	SendHttpRequest(Request, SendData);
}

void USaveManager::LoadInitInfo(const FString& Sub)
{
	UE_LOG(LogAuraDS, Log, TEXT("SaveManager : LoadInitInfo!"));

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::LoadInitInfo_Response);

	check(APIData);
	FAPIRequestSendData SendData(DedicatedServerTags::SaveDataAPI::LoadInitInfo, TEXT("POST"));
	SendData.Params.Add(TEXT("sub"), Sub);

	SendHttpRequest(Request, SendData);
}

void USaveManager::SaveGame_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogAuraDS, Log, TEXT("USaveManager::SaveGame_Response!"));

	if (!bWasSuccessful)
	{
		UE_LOG(LogAuraDS, Log, TEXT("----- Save UnSuccessful!"));
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			UE_LOG(LogAuraDS, Log, TEXT("----- Save UnSuccessful!"));
			return;
		}

		// Successful Response

		UE_LOG(LogAuraDS, Log, TEXT("SaveGame Succsessful!"));
	}
}

void USaveManager::LoadGame_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogAuraDS, Log, TEXT("USaveManager::LoadGame_Response!"));

	if (!bWasSuccessful)
	{
		UE_LOG(LogAuraDS, Log, TEXT("LoadGame_Response, NOT Successful"));
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			UE_LOG(LogAuraDS, Log, TEXT("LoadGame_Response, Have Errors"));
			return;
		}

		// Successful Response

		UE_LOG(LogAuraDS, Log, TEXT("=== LoadGame Succsessful! ==="));

		UAuraSaveData* SaveData = ConvertRetrieveDataToStruct(JsonObject);
		const FString& Sub = JsonObject->GetStringField(TEXT("Sub"));

		if (SaveData && !Sub.IsEmpty())
		{
			if (RetrieveCallbacksMap.Contains(Sub))
			{
				RetrieveCallbacksMap[Sub].ExecuteIfBound(SaveData);
				RetrieveCallbacksMap.Remove(Sub);
			}
		}
		else
		{
			UE_LOG(LogAuraDS, Log, TEXT("Response Data Is Wrong, %s"), *FString(__FUNCTION__));
		}
	}
	else
	{
		UE_LOG(LogAuraDS, Log, TEXT("Deserialize Failed!"));
	}
}

void USaveManager::LoadInitInfo_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogAuraDS, Log, TEXT("USaveManager::LoadInitInfo_Response"));

	if (!bWasSuccessful)
		SaveStatusMessageDelegate.Broadcast(HTTPStatusMessage::SomethingWrong, true);

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			SaveStatusMessageDelegate.Broadcast(HTTPStatusMessage::SomethingWrong, true);
			return;
		}

		FPlayerInfoSaveData PlayerInfoData;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &PlayerInfoData);

		// < Check bNewPlayer >
		// if bNewPlayer is true, 
		// then, this user has never saved Datas. 
		// so there's no data of Attributes, Inventory, etc.. 

		bool bNewPlayer = false;
		if (JsonObject->HasTypedField<EJson::Boolean>(TEXT("bNewPlayer")))
		{
			bNewPlayer = JsonObject->GetBoolField(TEXT("bNewPlayer"));
		}

		if (UDS_LocalPlayerSubsystem* Subsystem = GetDSLocalPlayerSubsystem())
		{
			//Subsystem->KeepLastMapName(PlayerInfoData.MapName);
			//Subsystem->KeepLastPlayerStartName(PlayerInfoData.PlayerStart);
			Subsystem->KeepIsNewPlayer(bNewPlayer);
		}

		KeepPlayersLastPos(PlayerInfoData.MapName, PlayerInfoData.PlayerStart);

		OnInitInfoLoaded.Broadcast(PlayerInfoData);
	}
}

void USaveManager::SetNotANewPlayer_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogAuraDS, Log, TEXT("SetNotANewPlayer_Response, Successful: %d"), bWasSuccessful);
}

void USaveManager::SetSaveDataParams(FAPIRequestSendData& OUTSendData)
{

}

UAuraSaveData* USaveManager::ConvertRetrieveDataToStruct(TSharedPtr<FJsonObject> JsonObject)
{
	UE_LOG(LogAuraDS, Log, TEXT("USaveManager::ConvertRetrieveDataToStruct"));

	const TArray<TSharedPtr<FJsonValue>>* JsonValueArray;
	if (!JsonObject->TryGetArrayField(TEXT("Response"), JsonValueArray))
	{
		UE_LOG(LogAuraDS, Log, TEXT("Can't Get Array Field, %s"), *FString(__FUNCTION__));
		return nullptr;
	}

	UAuraSaveData* SaveData = NewObject<UAuraSaveData>();
	
	for (TSharedPtr<FJsonValue> Entry : *JsonValueArray)
	{
		const TSharedPtr<FJsonObject> EntryObject = Entry->AsObject();
		if (!EntryObject.IsValid())
			continue;

		const FString& TableName = EntryObject->GetStringField(TEXT("tableName"));
		TSharedPtr<FJsonObject> DataObject = EntryObject->GetObjectField(TEXT("data"));
	
		if (TableName == PLAYER_INFO_TABLE)
		{
			FJsonObjectConverter::JsonObjectToUStruct(DataObject.ToSharedRef(), &SaveData->PlayerData);
			SaveData->PlayerData.Dump();
		}
		else if (TableName == ATTRIBUTES_TABLE)
		{
			FJsonObjectConverter::JsonObjectToUStruct(DataObject.ToSharedRef(), &SaveData->AttributesData);
			SaveData->AttributesData.Dump();
		}
		else if (TableName == INVENTORY_TABLE)
		{
			FJsonObjectConverter::JsonObjectToUStruct(DataObject.ToSharedRef(), &SaveData->InventoryData);
			SaveData->InventoryData.Dump();
		}
		else if (TableName == SKILLS_TABLE)
		{
			for (const auto& Pair : DataObject->Values)
			{
				if (!Pair.Key.IsNumeric() || Pair.Value->Type != EJson::Object)
					continue;

				const TSharedPtr<FJsonObject> SkillData = Pair.Value->AsObject();
				if (!SkillData.IsValid())
					continue;

				FSkillSaveData SkillSaveData;
				FJsonObjectConverter::JsonObjectToUStruct(SkillData.ToSharedRef(), &SkillSaveData);
				SaveData->SavedSkills.Add(SkillSaveData);
				SkillSaveData.Dump();
			}
		}
	}

	return SaveData;
}

void USaveManager::KeepPlayersLastPos(const FString& LastMapName, const FString& LastPlayerStartName)
{
	check(GameInstanceSubsystem);
	GameInstanceSubsystem->KeepLastMapName(LastMapName);
	GameInstanceSubsystem->KeepLastPlayerStartName(LastPlayerStartName);
}

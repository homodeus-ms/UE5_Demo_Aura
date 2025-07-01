// Fill out your copyright notice in the Description page of Project Settings.


#include "HTTP/PortalManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HttpModule.h"
#include "DedicatedServer/DedicatedServerTags.h"
#include "Common/DedicatedServer.h"
#include "JsonObjectConverter.h"
#include "Player/DS_LocalPlayerSubsystem.h"
#include "DedicatedServer/HTTPRequestTypes.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "Interfaces/HUDManageInterface.h"
#include "Interfaces/SaveManageInterface.h"
#include "Player/DS_PlayerState.h"


void UPortalManager::SignIn(const FString& UserName, const FString& Password)
{
	SignInStatusMessageDelegate.Broadcast(TEXT("로그인 중입니다..."), false);
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::SignIn_Response);

	check(APIData);
	FAPIRequestSendData SendData(DedicatedServerTags::PortalAPI::SignIn, TEXT("POST"));
	SendData.Params.Add(TEXT("username"), UserName);
	SendData.Params.Add(TEXT("password"), Password);

	CachedUserName = UserName;

	SendHttpRequest(Request, SendData);
}

void UPortalManager::SignUp(const FString& UserName, const FString& Password, const FString& Email)
{
	SignUpStatusMessageDelegate.Broadcast(TEXT("계정 생성중입니다..."), false);
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::SignUp_Response);

	check(APIData);
	FAPIRequestSendData SendData(DedicatedServerTags::PortalAPI::SignUp, TEXT("POST"));
	SendData.Params.Add(TEXT("username"), UserName);
	SendData.Params.Add(TEXT("password"), Password);
	SendData.Params.Add(TEXT("email"), Email);

	CachedUserName = UserName;

	SendHttpRequest(Request, SendData);
}

void UPortalManager::ConfirmEmail(const FString& ConfrimCode)
{
	ConfirmEmailStatusMessageDelegate.Broadcast(TEXT("계정 확인중입니다..."), false);
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::Confirm_Response);

	check(APIData);
	FAPIRequestSendData SendData(DedicatedServerTags::PortalAPI::ConfirmSignUp, TEXT("PUT"));
	SendData.Params.Add(TEXT("username"), LastSignUpUserName);
	SendData.Params.Add(TEXT("confirmationCode"), ConfrimCode);

	LastSignUpUserName = FString();

	SendHttpRequest(Request, SendData);
}

void UPortalManager::QuitGame()
{
	APlayerController* PlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(PlayerController))
		UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, false);
}


void UPortalManager::RefreshToken(const FString& RefreshToken)
{
	check(APIData);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::RefreshToken_Response);

	FAPIRequestSendData SendData(DedicatedServerTags::PortalAPI::SignIn, TEXT("POST"));
	SendData.Params.Add(TEXT("refreshToken"), RefreshToken);

	SendHttpRequest(Request, SendData);
}

void UPortalManager::SignOut(const FString& AccessToken)
{
	check(APIData);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::SignOut_Response);

	FAPIRequestSendData SendData(DedicatedServerTags::PortalAPI::SignOut, TEXT("POST"));
	SendData.Params.Add(TEXT("accessToken"), AccessToken);

	SendHttpRequest(Request, SendData);
}

void UPortalManager::SignIn_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		SignInStatusMessageDelegate.Broadcast(HTTPStatusMessage::SomethingWrong, true);
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			SignInStatusMessageDelegate.Broadcast(HTTPStatusMessage::SomethingWrong, true);
			return;
		}

		FDSInitiateAuthResponse AuthResponse;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &AuthResponse);

		if (UDS_LocalPlayerSubsystem* Subsystem = GetDSLocalPlayerSubsystem())
		{
			Subsystem->InitiateToken(AuthResponse.AuthenticationResult, this);
			Subsystem->KeepUserName(CachedUserName);
			Subsystem->KeepEmail(AuthResponse.Email);
			Subsystem->KeepSub(AuthResponse.Sub);
		}

		APlayerController* PlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (PlayerController)
		{
			if (IHUDManageInterface* Interface = Cast<IHUDManageInterface>(PlayerController->GetHUD()))
			{
				Interface->OnSignIn();
			}
		}

		CachedUserName = FString();

		OnSignInSucceeded.Broadcast();
	}
}

void UPortalManager::SignUp_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
		SignUpStatusMessageDelegate.Broadcast(HTTPStatusMessage::SomethingWrong, true);

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			SignUpStatusMessageDelegate.Broadcast(HTTPStatusMessage::SomethingWrong, true);
			return;
		}

		LastSignUpUserName = CachedUserName;
		CachedUserName = FString();
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &LastSignUpResponse);

		LastSignUpResponse.Dump();

		OnSignUpSucceeded.Broadcast();
	}
}

void UPortalManager::Confirm_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		ConfirmEmailStatusMessageDelegate.Broadcast(HTTPStatusMessage::SomethingWrong, true);
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			if (JsonObject->HasField(TEXT("name")) &&
				JsonObject->GetStringField(TEXT("name")).Equals(TEXT("CodeMismatchException")))
			{
				ConfirmEmailStatusMessageDelegate.Broadcast(TEXT("코드가 다릅니다!"), true);
				return;
			}

			ConfirmEmailStatusMessageDelegate.Broadcast(HTTPStatusMessage::SomethingWrong, true);
			return;
		}

		// Confirm Succssess

		if (UDS_LocalPlayerSubsystem* Subsystem = GetDSLocalPlayerSubsystem())
		{
			
		}

		OnConfirmEmailSucceeded.Broadcast();
	}
}

void UPortalManager::SignOut_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
		return;

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
			return;

		// Successful Response
		APlayerController* PlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (PlayerController)
		{
			if (IHUDManageInterface* Interface = Cast<IHUDManageInterface>(PlayerController->GetHUD()))
			{
				Interface->OnSignOut();
			}
		}
	}
}

void UPortalManager::RefreshToken_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
		return;

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
			return;

		// Successful Response
		FDSInitiateAuthResponse InitiateAuthResponse;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &InitiateAuthResponse);

		if (UDS_LocalPlayerSubsystem* Subsystem = GetDSLocalPlayerSubsystem())
		{
			Subsystem->UpdateToken(InitiateAuthResponse.AuthenticationResult);
		}
	}
}

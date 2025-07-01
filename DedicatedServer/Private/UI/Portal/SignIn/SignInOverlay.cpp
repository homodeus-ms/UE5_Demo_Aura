// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignInOverlay.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Portal/SignIn/SignInPage.h"
#include "UI/Portal/SignIn/SignUpPage.h"
#include "UI/Portal/SignIn/ConfirmEmailPage.h"
#include "UI/Portal/SignIn/SuccessSignUpPage.h"
#include "UI/Common/DS_Button.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "HTTP/PortalManager.h"

void USignInOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	check(PortalManager);
	
	SignInPage->Button_SignIn->Button->OnClicked.AddDynamic(this, &ThisClass::OnSignInButtonClicked);
	SignInPage->Button_SignUp->Button->OnClicked.AddDynamic(this, &ThisClass::ShowSignUpPage);
	SignInPage->Button_Quit->Button->OnClicked.AddDynamic(PortalManager, &UPortalManager::QuitGame);
	PortalManager->SignInStatusMessageDelegate.AddDynamic(SignInPage, &USignInPage::SetStatusMesage);
	PortalManager->OnSignInSucceeded.AddDynamic(this, &ThisClass::OnSignInSucceeded);

	SignUpPage->Button_SignUp->Button->OnClicked.AddDynamic(this, &ThisClass::OnSignUpButtonClicked);
	SignUpPage->Button_Back->Button->OnClicked.AddDynamic(this, &ThisClass::ShowSignInPage);
	PortalManager->SignUpStatusMessageDelegate.AddDynamic(SignUpPage, &USignUpPage::SetStatusMessage);
	PortalManager->OnSignUpSucceeded.AddDynamic(this, &ThisClass::OnSignUpSucceeded);

	ConfirmEmailPage->Button_Confirm->Button->OnClicked.AddDynamic(this, &ThisClass::OnConfirmButtonClicked);
	ConfirmEmailPage->Button_Back->Button->OnClicked.AddDynamic(this, &ThisClass::ShowSignUpPage);
	PortalManager->ConfirmEmailStatusMessageDelegate.AddDynamic(ConfirmEmailPage, &UConfirmEmailPage::SetStatusMessage);
	PortalManager->OnConfirmEmailSucceeded.AddDynamic(this, &ThisClass::OnConfirmSucceeded);

	SuccessSignUpPage->Button_OK->Button->OnClicked.AddDynamic(this, &ThisClass::ShowSignInPage);
}

void USignInOverlay::OnSignInButtonClicked()
{
	const FString& UserName = SignInPage->TextBox_Name->GetText().ToString();
	const FString& Password = SignInPage->TextBox_Password->GetText().ToString();

	PortalManager->SignIn(UserName, Password);
}

void USignInOverlay::OnSignUpButtonClicked()
{
	const FString& UserName = SignUpPage->TextBox_UserName->GetText().ToString();
	const FString& Password = SignUpPage->TextBox_Password->GetText().ToString();
	const FString& Email = SignUpPage->TextBox_Email->GetText().ToString();

	PortalManager->SignUp(UserName, Password, Email);
}

void USignInOverlay::OnConfirmButtonClicked()
{
	PortalManager->ConfirmEmail(ConfirmEmailPage->TextBox_ConfirmCode->GetText().ToString());
}

void USignInOverlay::OnSignInSucceeded()
{
	SignInPage->ClearTextBoxes();
}

void USignInOverlay::OnSignUpSucceeded()
{
	SignUpPage->ClearTextBoxes();
	const FString& Email = PortalManager->GetLastSignUpResponse().CodeDeliveryDetails.Destination;
	ConfirmEmailPage->TextBlock_Email->SetText(FText::FromString(Email));
	ShowConfirmPage();
}

void USignInOverlay::OnConfirmSucceeded()
{
	ConfirmEmailPage->ClearTextBoxes();
	ShowSuccessPage();
}

void USignInOverlay::ShowSignInPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SignInPage));
	WidgetSwitcher->SetActiveWidget(SignInPage);
}

void USignInOverlay::ShowSignUpPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SignUpPage));
	WidgetSwitcher->SetActiveWidget(SignUpPage);
}

void USignInOverlay::ShowConfirmPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(ConfirmEmailPage));
	WidgetSwitcher->SetActiveWidget(ConfirmEmailPage);
}

void USignInOverlay::ShowSuccessPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SuccessSignUpPage));
	WidgetSwitcher->SetActiveWidget(SuccessSignUpPage);
}
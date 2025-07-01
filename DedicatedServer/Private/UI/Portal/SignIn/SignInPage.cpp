// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignInPage.h"
#include "UI/Common/DS_StatusMessageWidget.h"
#include "Components/EditableTextBox.h"
#include "UI/Common/DS_Button.h"

void USignInPage::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateStatusMessage(TEXT("아이디와 비밀번호를 입력하세요..."));
	TextBox_Name->OnTextChanged.AddDynamic(this, &ThisClass::OnTextBoxChanged);
	TextBox_Password->OnTextChanged.AddDynamic(this, &ThisClass::OnTextBoxChanged);
	Button_SignIn->SetButtonEnable(false);
}

void USignInPage::OnTextBoxChanged(const FText& Text)
{
	const FString& Name = TextBox_Name->GetText().ToString();
	bool bValidName = !Name.IsEmpty() && Name.Len() >= 4 && Name.Len() <= 12;
	
	const FString& Password = TextBox_Password->GetText().ToString();
	bool bValidPassword = !Password.IsEmpty() && Password.Len() >= 8;

	Button_SignIn->SetButtonEnable(bValidName && bValidPassword);
}

void USignInPage::SetStatusMesage(const FString& Message, bool bResetButton)
{
	Super::UpdateStatusMessage(Message);
	Button_SignIn->SetButtonEnable(bResetButton);
}

void USignInPage::ClearTextBoxes()
{
	TextBox_Name->SetText(FText::GetEmpty());
	TextBox_Password->SetText(FText::GetEmpty());
	ClearStatusMessage();
}

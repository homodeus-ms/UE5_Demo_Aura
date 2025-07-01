// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/ConfirmEmailPage.h"
#include "UI/Common/DS_Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void UConfirmEmailPage::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateStatusMessage(TEXT("이메일로 전송된 6자리 코드를 입력하세요..."));
	Button_Confirm->SetButtonEnable(false);
	TextBox_ConfirmCode->OnTextChanged.AddDynamic(this, &ThisClass::OnTextBoxChanged);
}

void UConfirmEmailPage::OnTextBoxChanged(const FText& Text)
{
	Button_Confirm->SetButtonEnable(false);

	const FString& ConfirmCode = TextBox_ConfirmCode->GetText().ToString();
	const FRegexPattern CodePattern(TEXT(R"(^[0-9]{6}$)"));
	FRegexMatcher CodeMatcher(CodePattern, ConfirmCode);

	if (ConfirmCode.Len() == 6 && CodeMatcher.FindNext())
		Button_Confirm->SetButtonEnable(true);
}

void UConfirmEmailPage::SetStatusMessage(const FString& Message, bool bResetButton)
{
	Super::UpdateStatusMessage(Message);

	Button_Confirm->SetButtonEnable(bResetButton);
}

void UConfirmEmailPage::ClearTextBoxes()
{
	ClearStatusMessage();
	TextBox_ConfirmCode->SetText(FText::GetEmpty());
	TextBlock_Email->SetText(FText::GetEmpty());
}
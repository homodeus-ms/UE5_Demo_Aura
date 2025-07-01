// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignUpPage.h"
#include "UI/Common/DS_StatusMessageWidget.h"
#include "Components/EditableTextBox.h"
#include "UI/Common/DS_Button.h"

void USignUpPage::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateStatusMessage(TEXT("이름과 비밀번호, 이메일 주소를 입력하세요..."));
	TextBox_UserName->OnTextChanged.AddDynamic(this, &ThisClass::OnTextBoxChanged);
	TextBox_Password->OnTextChanged.AddDynamic(this, &ThisClass::OnTextBoxChanged);
	TextBox_ConfirmPassword->OnTextChanged.AddDynamic(this, &ThisClass::OnTextBoxChanged);
	TextBox_Email->OnTextChanged.AddDynamic(this, &ThisClass::OnTextBoxChanged);

	Button_SignUp->SetButtonEnable(false);
}

void USignUpPage::OnTextBoxChanged(const FText& Text)
{
	Button_SignUp->SetButtonEnable(false);

	const FString& UserName = TextBox_UserName->GetText().ToString();
	if (!IsValidUserName(UserName))
	{
		return;
	}

	UpdateStatusMessage(TEXT("비밀번호를 입력하세요. 비밀번호는 8글자 이상, 영문(대문자, 소문자), 숫자, 특수문자가 각각 하나 이상 들어가야 합니다..."));
	
	const FString& Password = TextBox_Password->GetText().ToString();
	if (!IsStrongPassword(Password))
	{
		return;
	}

	const FString& ConfirmPassword = TextBox_ConfirmPassword->GetText().ToString();
	if (ConfirmPassword.IsEmpty())
	{
		UpdateStatusMessage(TEXT("비밀번호를 한 번 더 입력하세요."));
		return;
	}

	if (!Password.Equals(ConfirmPassword))
	{
		UpdateStatusMessage(TEXT("비밀번호가 일치하지 않습니다..."));
		return;
	}

	const FString& Email = TextBox_Email->GetText().ToString();
	if (!IsValidEmail(Email))
	{
		UpdateStatusMessage(TEXT("사용하시는 이메일 주소를 입력하세요. 이메일로 확인 코드가 발송됩니다..."));
		return;
	}

	// Success
	UpdateStatusMessage(TEXT("입력된 정보를 확인하고 Sign Up 버튼을 클릭하세요."));
	Button_SignUp->SetButtonEnable(true);
}

bool USignUpPage::IsValidUserName(const FString& UserName)
{
	const FRegexPattern UserNamePattern(TEXT(R"(^[A-Za-z0-9]{4,12}$)"));
	FRegexMatcher UserNameMatcher(UserNamePattern, UserName);
	if (UserNameMatcher.FindNext())
		return true;

	UpdateStatusMessage(TEXT("이름은 4 ~ 12 글자로, 영문, 숫자만 사용 가능합니다..."));
	return false;
}

bool USignUpPage::IsStrongPassword(const FString& Password)
{
	if (Password.Len() < 8)
	{
		UpdateStatusMessage(TEXT("비밀번호는 8글자 이상이어야 합니다..."));
		return false;
	}

	bool HasNumber = false;
	bool HasSpecialChar = false;
	bool HasLowerCase = false;
	bool HasUpperCase = false;

	for (TCHAR c : Password)
	{
		if ((uint32)c < 33 || (uint32)c > 126)
		{
			UpdateStatusMessage(TEXT("비밀번호는 영문 대소문자, 숫자, 특수문자만 사용할 수 있습니다..."));
			return false;
		}

		if (!HasNumber && (c >= '0' && c <= '9'))
		{
			HasNumber = true;
		}
		else if (!HasLowerCase && (c >= 'a' && c <= 'z'))
		{
			HasLowerCase = true;
		}
		else if (!HasUpperCase && (c >= 'A' && c <= 'Z'))
		{
			HasUpperCase = true;
		}
		else if (!HasSpecialChar && !FChar::IsAlnum(c) && !FChar::IsWhitespace(c))
		{
			HasSpecialChar = true;
		}

		if (HasNumber && HasSpecialChar && HasLowerCase && HasUpperCase)
			break;
	}

	if (!HasNumber)
	{
		UpdateStatusMessage(TEXT("비밀 번호에 숫자가 하나 이상 있어야 합니다."));
		return false;
	}
	else if (!HasSpecialChar)
	{
		UpdateStatusMessage(TEXT("비밀 번호에 특수 문자가 하나 이상 있어야 합니다."));
		return false;
	}
	else if (!HasLowerCase)
	{
		UpdateStatusMessage(TEXT("비밀 번호에 영문 소문자가 하나 이상 있어야 합니다."));
		return false;
	}
	else if (!HasUpperCase)
	{
		UpdateStatusMessage(TEXT("비밀 번호에 영문 대문자가 하나 이상 있어야 합니다."));
		return false;
	}

	return true;
}

bool USignUpPage::IsValidEmail(const FString& Email)
{
	const FRegexPattern EmailPattern(TEXT(R"((^[^\s@]+@[^\s@]+\.[^\s@]{2,}$))"));
	FRegexMatcher Matcher(EmailPattern, Email);

	return Matcher.FindNext();
}

void USignUpPage::SetStatusMessage(const FString& Message, bool bResetButton)
{
	Super::UpdateStatusMessage(Message);

	Button_SignUp->SetButtonEnable(bResetButton);
}


void USignUpPage::ClearTextBoxes()
{
	Button_SignUp->SetButtonEnable(false);
	TextBox_UserName->SetText(FText::GetEmpty());
	TextBox_Password->SetText(FText::GetEmpty());
	TextBox_ConfirmPassword->SetText(FText::GetEmpty());
	TextBox_Email->SetText(FText::GetEmpty());

	ClearStatusMessage();
}

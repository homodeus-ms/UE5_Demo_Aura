// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Portal/PageWidgetBase.h"
#include "SignUpPage.generated.h"

class UDS_Button;
class UEditableTextBox;

/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API USignUpPage : public UPageWidgetBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetStatusMessage(const FString& Message, bool bResetButton);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_UserName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Password;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_ConfirmPassword;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Email;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDS_Button> Button_SignUp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDS_Button> Button_Back;

	void ClearTextBoxes();

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnTextBoxChanged(const FText& Text);

	bool IsValidUserName(const FString& UserName);
	bool IsStrongPassword(const FString& Password);
	bool IsValidEmail(const FString& Email);
};

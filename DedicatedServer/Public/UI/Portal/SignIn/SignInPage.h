// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Portal/PageWidgetBase.h"
#include "SignInPage.generated.h"

class UDS_Button;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API USignInPage : public UPageWidgetBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void SetStatusMesage(const FString& Message, bool bResetButton);
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Name;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Password;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDS_Button> Button_SignIn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDS_Button> Button_SignUp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDS_Button> Button_Quit;

	void ClearTextBoxes();

protected:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void OnTextBoxChanged(const FText& Text);

	
};

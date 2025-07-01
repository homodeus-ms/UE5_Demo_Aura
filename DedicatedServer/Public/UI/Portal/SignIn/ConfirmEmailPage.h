// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Portal/PageWidgetBase.h"
#include "ConfirmEmailPage.generated.h"

class UTextBlock;
class UDS_Button;
class UEditableTextBox;

/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API UConfirmEmailPage : public UPageWidgetBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetStatusMessage(const FString& Message, bool bResetButton);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Email;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_ConfirmCode;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDS_Button> Button_Confirm;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDS_Button> Button_Back;

	void ClearTextBoxes();

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnTextBoxChanged(const FText& Text);
};

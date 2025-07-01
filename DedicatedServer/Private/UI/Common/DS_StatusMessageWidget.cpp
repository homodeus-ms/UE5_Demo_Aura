// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/DS_StatusMessageWidget.h"
#include "Components/TextBlock.h"

const FString& UDS_StatusMessageWidget::GetText() const
{
	return TextBlock_Message->GetText().ToString();
}

void UDS_StatusMessageWidget::UpdateStatusMessage(const FString& Message)
{
	TextBlock_Message->SetText(FText::FromString(Message));
}

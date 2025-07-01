// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/PageWidgetBase.h"
#include "Components/TextBlock.h"

void UPageWidgetBase::UpdateStatusMessage(const FString& Message)
{
	TextBlock_StatusMessage->SetText(FText::FromString(Message));
}

void UPageWidgetBase::ClearStatusMessage()
{
	TextBlock_StatusMessage->SetText(FText::GetEmpty());
}

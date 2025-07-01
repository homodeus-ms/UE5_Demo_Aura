// Fill out your copyright notice in the Description page of Project Settings.

#include "Chat/UI/ChatInputBox.h"
#include "Components/EditableTextBox.h"


void UChatInputBox::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = true;
}



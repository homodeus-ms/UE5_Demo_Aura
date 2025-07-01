// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/DS_Button.h"

void UDS_Button::SetButtonEnable(bool bEnable)
{
	if (Button)
		Button->SetIsEnabled(bEnable);
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SuccessSignUpPage.generated.h"

class UDS_Button;

/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API USuccessSignUpPage : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDS_Button> Button_OK;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PageWidgetBase.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API UPageWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateStatusMessage(const FString& Message);
	void ClearStatusMessage();

protected:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_StatusMessage;
};

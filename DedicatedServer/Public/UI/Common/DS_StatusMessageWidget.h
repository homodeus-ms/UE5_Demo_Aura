// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DS_StatusMessageWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API UDS_StatusMessageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	const FString& GetText() const;
	void UpdateStatusMessage(const FString& Message);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Message;
};

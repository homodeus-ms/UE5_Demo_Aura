// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/AuraUserWidget.h"
#include "ChatInputBox.generated.h"

class UTextBlock;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class AURA_API UChatInputBox : public UAuraUserWidget
{
	GENERATED_BODY()
	
public:

protected:
	virtual void NativeConstruct() override;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_MessageBox;
};

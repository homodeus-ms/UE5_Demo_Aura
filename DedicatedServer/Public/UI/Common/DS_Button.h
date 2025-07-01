// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "DS_Button.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API UDS_Button : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetButtonEnable(bool bEnable);

	template<typename UserClass>
	void BindFuncToButtonClicked(UserClass* InUserObject, typename TMemFunPtrType<false, UserClass, void()>::Type InFunc);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UButton> Button;

protected:
	

	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTextBlock> TextBlock_ButtonName;
	
};

template<typename UserClass>
void UDS_Button::BindFuncToButtonClicked(UserClass* InUserObject, typename TMemFunPtrType<false, UserClass, void()>::Type InFunc)
{
	if (Button)
		Button->OnClicked.AddDynamic(InUserObject, InFunc);
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignInOverlay.generated.h"

class USignInPage;
class USignUpPage;
class UConfirmEmailPage;
class USuccessSignUpPage;
class UWidgetSwitcher;
class UPortalManager;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVER_API USignInOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetManager(UPortalManager* InPortalManager) { PortalManager = InPortalManager; }

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USignInPage> SignInPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USignUpPage> SignUpPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UConfirmEmailPage> ConfirmEmailPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USuccessSignUpPage> SuccessSignUpPage;

	UPROPERTY()
	TObjectPtr<UPortalManager> PortalManager;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void ShowSignInPage();

	UFUNCTION()
	void ShowSignUpPage();

	UFUNCTION()
	void ShowConfirmPage();

	UFUNCTION()
	void ShowSuccessPage();

	UFUNCTION()
	void OnSignInButtonClicked();

	UFUNCTION()
	void OnSignUpButtonClicked();

	UFUNCTION()
	void OnConfirmButtonClicked();

	UFUNCTION()
	void OnSignInSucceeded();

	UFUNCTION()
	void OnSignUpSucceeded();

	UFUNCTION()
	void OnConfirmSucceeded();
};

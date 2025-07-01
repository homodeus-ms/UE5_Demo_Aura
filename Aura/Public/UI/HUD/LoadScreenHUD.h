// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoadScreenHUD.generated.h"

/**
 * 
 */

class UMVVM_LoadScreen;

UCLASS()
class AURA_API ALoadScreenHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadScreenWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class ULoadScreenWidget> LoadScreenWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "MVVM")
	TSubclassOf<UMVVM_LoadScreen> LoadScreenVMClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_LoadScreen> LoadScreenVM;

protected:
	virtual void BeginPlay() override;

	
};

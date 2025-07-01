// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/LoadScreenHUD.h"
#include "UI/Widgets/LoadScreenWidget.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"

void ALoadScreenHUD::BeginPlay()
{
	/*LoadScreenVM = NewObject<UMVVM_LoadScreen>(this, LoadScreenVMClass);
	LoadScreenVM->InitLoadSlotVMs();

	LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();
	LoadScreenWidget->OnBlueprintWidgetCreated();
	
	LoadScreenVM->LoadSlotData();*/
}

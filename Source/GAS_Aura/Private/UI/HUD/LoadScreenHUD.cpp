// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/LoadScreenHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/LoadScreenWidget.h"
#include "UI/ViewMode/MVVM_LoadScreen.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	// 创建所有 ViewModel
	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModelClass);
	LoadScreenViewModel->InitializeLoadSlots();

	// 创建 LoadScreen 
	LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);

	LoadScreenWidget->AddToViewport();

	// 通知 LoadScreen 的子组件(三个存档Slot) 去设置ViewModel
	LoadScreenWidget->BlueprintInitializeWidget();
}

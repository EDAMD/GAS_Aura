// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewMode/MVVM_LoadScreen.h"
#include "UI/ViewMode/MVVM_LoadSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Game/AuraGameModeBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "Game/AuraGameInstance.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);

	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);

	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);

	SetNumLoadSlots(LoadSlots.Num());
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));

	LoadSlots[Slot]->SetMapName(AuraGameMode->DefaultMapName);
	LoadSlots[Slot]->SetPlayerName(EnteredName);
	LoadSlots[Slot]->SlotStatus = ESaveSlotStatus::Taken;
	LoadSlots[Slot]->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;

	AuraGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializeSlot();

	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance());
	AuraGameInstance->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
	AuraGameInstance->LoadSlotName = LoadSlots[Slot]->GetLoadSlotName();
	AuraGameInstance->LoadSlotIndex = LoadSlots[Slot]->SlotIndex;
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	SlotSelected.Broadcast();
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == Slot)
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(false);
			
		}
		else
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(true);
		}
	}
	SelectedSlot = LoadSlots[Slot];
}

void UMVVM_LoadScreen::LoadData()
{
	// 从磁盘文件中获取存档, 使用存档内容初始化到内存中
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* SaveObject = AuraGameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);

		const FString PlayerName = SaveObject->PlayerName;
		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;
		const FString MapName = SaveObject->MapName;

		LoadSlot.Value->SetPlayerName(PlayerName);
		LoadSlot.Value->SlotStatus = SaveSlotStatus;
		LoadSlot.Value->SetMapName(MapName);
		LoadSlot.Value->PlayerStartTag = SaveObject->PlayerStartTag;
		// 根据 SlotStatus 切换 Slot Switcher
		LoadSlot.Value->InitializeSlot();
	}
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		AAuraGameModeBase::DeleteSlot(SelectedSlot->GetLoadSlotName(), SelectedSlot->SlotIndex);

		// 切换 WidgetSwitcher 到 Vacant 页面(Active Widget Index 0)
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();

		// 通知 SelectButton Enable
		SelectedSlot->EnableSelectSlotButton.Broadcast(true);
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance());
	AuraGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
	AuraGameInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();
	AuraGameInstance->LoadSlotIndex = SelectedSlot->SlotIndex;
	
	if (IsValid(SelectedSlot))
	{
		AuraGameMode->TravelToMap(SelectedSlot);
	}
}

void UMVVM_LoadScreen::SetNumLoadSlots(int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InNumLoadSlots);
}

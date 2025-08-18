// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewMode/MVVM_LoadScreen.h"
#include "UI/ViewMode/MVVM_LoadSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Game/AuraGameModeBase.h"
#include "Game/LoadScreenSaveGame.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetPlayerName(FString("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);

	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetPlayerName(FString("LoadSlot_1"));
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);

	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetPlayerName(FString("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);
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
	AuraGameMode->SaveSlotData(LoadSlots[Slot], Slot);

	LoadSlots[Slot]->InitializeSlot();
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
		ULoadScreenSaveGame* SaveObject = AuraGameMode->GetSaveSlotData(LoadSlot.Value->LoadSlotName, LoadSlot.Key);

		const FString PlayerName = SaveObject->PlayerName;
		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;

		LoadSlot.Value->SetPlayerName(PlayerName);
		LoadSlot.Value->SlotStatus = SaveSlotStatus;
		// 根据 SlotStatus 切换 Slot Switcher
		LoadSlot.Value->InitializeSlot();
	}
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		AAuraGameModeBase::DeleteSlot(SelectedSlot->LoadSlotName, SelectedSlot->SlotIndex);

		// 切换 WidgetSwitcher 到 Vacant 页面(Active Widget Index 0)
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();

		// 通知 SelectButton Enable
		SelectedSlot->EnableSelectSlotButton.Broadcast(true);
	}
}

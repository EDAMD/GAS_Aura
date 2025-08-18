// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewMode/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	// TODO: Check slot status based on loaded data.
	const int32 WidgetSwitcherIndex = SlotStatus.GetIntValue();
	SetWidgetSwitcherIndex.Broadcast(WidgetSwitcherIndex);
}

void UMVVM_LoadSlot::SetPlayerName(FString InPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

void UMVVM_LoadSlot::SetMapName(FString InMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
}

 // Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BoradcastInitialValues()
{
	BroadcastAbilityInfo();

	SpellPointsDelegate.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{

	GetAuraASC()->AbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;

			AbilityInfoDelegate.Broadcast(Info);
		}
	);

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 NewValue)
		{
			SpellPointsDelegate.Broadcast(NewValue);
		}
	);
}

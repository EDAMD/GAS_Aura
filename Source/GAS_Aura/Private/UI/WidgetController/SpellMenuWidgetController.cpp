 // Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"
#include "AuraGameplayTags.h"

void USpellMenuWidgetController::BoradcastInitialValues()
{
	BroadcastAbilityInfo();

	SpellPointsDelegate.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	// 当技能变化时, 同时更改 按钮 以及 Spell Globe 的状态
	GetAuraASC()->AbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
		{
			// 更改 SpendPoints 和 Equip Button 的 状态
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;

				bool bEnableSpendPoints = false;
				bool bEnableEquipped = false;
				ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquipped);
				FString Description;
				FString NextLevelDescription;
				GetAuraASC()->GetDescriptionByAbilityTag(AbilityTag, Description, NextLevelDescription);
				SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquipped, Description, NextLevelDescription);
			}

			// 更改技能 Icon 和 Background
			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;

				AbilityInfoDelegate.Broadcast(Info);
			}
		}
	);

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 NewValue)
		{
			SpellPointsDelegate.Broadcast(NewValue);
			CurrentSpellPoints = NewValue;

			bool bEnableSpendPoints = false;
			bool bEnableEquipped = false;
			ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquipped);
			FString Description;
			FString NextLevelDescription;
			GetAuraASC()->GetDescriptionByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquipped, Description, NextLevelDescription);
		}
	);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	// 判断当前选中能力 是否有效
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTagExact(GameplayTags.Abilities_None);
	FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;

	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraASC()->GetStatusFromSpec(*AbilitySpec);
	}
	
	// 设置当前选中 Ability
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;

	bool bEnableSpendPoints = false;
	bool bEnableEquipped = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquipped);
	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionByAbilityTag(AbilityTag, Description, NextLevelDescription);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquipped, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointsButtonPressed()
{
	if (GetAuraASC())
	{
		GetAuraASC()->ServerSpendSpellPoints(SelectedAbility.Ability);
	}	
}

void USpellMenuWidgetController::GlobeDeselect()
{
	SelectedAbility.Ability = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FAuraGameplayTags::Get().Abilities_Status_Locked;

	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& StatusTag, int32 SpellPoints, bool& bShouldSpendPointsButtonEnabled, bool& bShouldEquippedButtonEnabled)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	bShouldEquippedButtonEnabled = false;
	bShouldSpendPointsButtonEnabled = false;

	if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEquippedButtonEnabled = true;
		if (SpellPoints > 0)
		{
			bShouldSpendPointsButtonEnabled = true;
		}
	}
	else if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldSpendPointsButtonEnabled = true;
		}
	}
	else if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEquippedButtonEnabled = true;
		if (SpellPoints > 0)
		{
			bShouldSpendPointsButtonEnabled = true;
		}
	}
}

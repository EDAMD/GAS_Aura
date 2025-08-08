// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "AuraGameplayTags.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquippedButtonEnabled, FString, Description, FString, NextLevelDescription);

struct FGameplayTag;

struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GAS_AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "GAS|PlayerState")
	FOnPlayerStateChangedSignature SpellPointsDelegate;

	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;

	virtual void BoradcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendPointsButtonPressed();

	UFUNCTION(BlueprintCallable)
	void GlobeDeselect();

private:
	static void ShouldEnableButtons(const FGameplayTag& StatusTag, int32 SpellPoints, bool& bShouldSpendPointsButtonEnabled, bool& bShouldEquippedButtonEnabled);

	FSelectedAbility SelectedAbility = { FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSpellPoints = 0;
};

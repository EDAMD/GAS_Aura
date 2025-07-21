// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAssetManager.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystemGlobals.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	UAuraAssetManager* AuraAssertManager = Cast<UAuraAssetManager>(GEngine->AssetManager.Get());
	return *AuraAssertManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializedNativeGameplayTags();

	// 这是使用 TargetDaTa 必须要干的
	UAbilitySystemGlobals::Get().InitGlobalData();
}

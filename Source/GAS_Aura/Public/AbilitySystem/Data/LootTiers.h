// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LootTiers|Spawning")
	TSubclassOf<AActor> LootClass;

	UPROPERTY(EditDefaultsOnly, Category = "LootTiers|Spawning")
	float ChanceToSpawn = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "LootTiers|Spawning")
	int32 MaxNumberToSpawn = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LootTiers|Spawning")
	bool bLootLevelOverride = true;
};

/**
 * 
 */
UCLASS()
class GAS_AURA_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LootTiers|Spawning")
	TArray<FLootItem> LootItems;

	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootItems();
	
};

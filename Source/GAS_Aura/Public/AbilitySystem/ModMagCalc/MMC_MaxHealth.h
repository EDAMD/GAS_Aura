// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class GAS_AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

	UMMC_MaxHealth(); // 1. 构造函数, 在构造函数中初始化要捕捉的值
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec)const override; // 2. 重写计算函数

private:
	FGameplayEffectAttributeCaptureDefinition VigorDef;   // 3. 捕捉值容器
};

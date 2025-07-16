// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GAS_AURA_API AAuraEnemyCharacter : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay()override;

	void InitAbilityActorInfo() override;

public:
	AAuraEnemyCharacter();
	virtual void HightlightActor() override;
	virtual void UnHightlightActor() override;

	UPROPERTY(BlueprintReadOnly)
	bool bHightlighted = false;


};

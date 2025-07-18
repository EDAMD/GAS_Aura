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

public:
	AAuraEnemyCharacter();

	/* Enemy Interface */
	virtual void HightlightActor() override;
	virtual void UnHightlightActor() override;
	/* End Enemy Interface */

	/* Combat Interface*/
	virtual int32 GetPlayerLevel()override;
	/* End Combat Interface*/

	UPROPERTY(BlueprintReadOnly)
	bool bHightlighted = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Default Class")
	int32 Level = 1;

protected:
	virtual void BeginPlay()override;

	void InitAbilityActorInfo() override;

};

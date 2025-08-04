// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemyCharacter.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class GAS_AURA_API AAuraEnemyCharacter : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemyCharacter();
	virtual void PossessedBy(AController* NewController)override;

	/* Enemy Interface */
	virtual void HightlightActor() override;
	virtual void UnHightlightActor() override;

	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() override;
	/* End Enemy Interface */

	/* Combat Interface*/
	virtual int32 GetPlayerLevel_Implementation()override;
	virtual void Die() override;
	virtual TArray<FTagedMontage> GetAttackMontages_Implementation() override;
	/* End Combat Interface*/

	UPROPERTY(BlueprintReadOnly)
	bool bHightlighted = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Default Class")
	int32 Level = 1;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	// 实现受伤眩晕效果
	void HitReactTagChanged(FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReaction = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;



protected:
	virtual void BeginPlay()override;

	void InitAbilityActorInfo() override;

	virtual void InitializedDefaultAttributes() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

};

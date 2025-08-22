// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Interaction/HighlightInterface.h"
#include "AuraEnemyCharacter.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class GAS_AURA_API AAuraEnemyCharacter : public AAuraCharacterBase, public IEnemyInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	AAuraEnemyCharacter();
	virtual void PossessedBy(AController* NewController)override;

	/* Highlight Interface */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	/* End Highlight Interface */

	/* Enemy Interface */
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() override;
	/* End Enemy Interface */

	/* Combat Interface*/
	virtual int32 GetPlayerLevel_Implementation()override;
	virtual void Die(const FVector DeathImpulse) override;
	virtual TArray<FTagedMontage> GetAttackMontages_Implementation() override;
	/* End Combat Interface*/

	void SetLevel(int32 InLevel) { Level = InLevel; }

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
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

protected:
	virtual void BeginPlay()override;

	void InitAbilityActorInfo() override;

	virtual void InitializedDefaultAttributes() override;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();

};

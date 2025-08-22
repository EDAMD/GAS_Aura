// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UNiagaraComponent;
class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class GAS_AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController)override;
	virtual void OnRep_PlayerState()override;

	/* Combat Interface*/
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector DeathImpulse) override;
	/* End Combat Interface*/

	/* Player Interface */
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation()const override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;

	virtual void ShowMagicCircle_Implementation(UMaterialInterface* InMaterial = nullptr) override;
	virtual void HideMagicCircle_Implementation() override;

	virtual void SaveProgress_Implementation(const FName& CheckPointTag) override;
	/* End Player Interface */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
	
	virtual void OnRep_Stunned() override;

	virtual void OnRep_Burned() override;

	UPROPERTY(EditDefaultsOnly)
	float DeathTime = 5.f;     // 死亡重生倒计时

	FTimerHandle DeathTimer;   // 死亡重生计时器

protected:
	// 从磁盘中加载信息
	void LoadProgress();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;


	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
};

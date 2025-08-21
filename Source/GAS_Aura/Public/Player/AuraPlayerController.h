// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UAuraAbilitySystemComponent;
struct FGameplayTag;
class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class USplineComponent;
class UDamageTextComponent;
class UNiagaraSystem;
class AMagicCircle;
class IHighlightInterface;

/**
 * 
 */
UCLASS()
class GAS_AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

	UFUNCTION(Client, Reliable)
	void ShowDamage(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* InMaterial = nullptr);

	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();



protected:
	virtual	void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	void ShiftPressed() { bShiftKeyDown = true; }
	void ShiftReleased() { bShiftKeyDown = false; }
	bool bShiftKeyDown = false;

	IHighlightInterface* LastActor;
	IHighlightInterface* ThisActor;
	FHitResult CursorHit;

	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	/* Click to Move Variable */
	FVector CachedDestination = FVector::Zero();			// 鼠标点击位置
	float FollowTime = 0.f;									// 按键按压时间
	float ShortPressThreshold = 0.5f;						// 短按判定阈值
	bool bAutoRunning = false;								// 是否自动移动
	bool bTargeting = false;								// 鼠标是否指向敌人

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;					// 自动移动访问半径

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;					// 样条曲线

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	/* Magic Circle */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCirCleClass;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;

	void UpdateMagicCircleLocation();
};

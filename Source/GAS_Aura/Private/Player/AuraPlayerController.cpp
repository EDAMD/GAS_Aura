// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"


AAuraPlayerController::AAuraPlayerController()
{
	// 响应服务器上变化的数据, 并发送到所有客户端
	bReplicates = true;

}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();	

	check(AuraContext);

	// 添加输入映射上下文
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	// 鼠标设置
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

	
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	 * 光标的射线追踪, 有以下几种情况
	 *  A. LastActor is null && ThisActor is null
	 *		- 光标没有碰撞到敌人, Do nothing
	 * 
	 *  B. LastActor is null && ThisActor is Valid
	 *		- 光标第一次碰到敌人, Hightlight ThisActor
	 * 
	 *  C. LastActor is Valid && ThisActor is null
	 *		- 光标离开敌人, UnHightlight LastActor
	 * 
	 *  D. Both Actors are valid, LastActor != ThisActor
	 *		- 光标突然移向另一个敌人, Hightlight ThisActor, UnHightlight LastActor
	 * 
	 *  E. Both Actors are valid, LastActor == ThisActor
	 *		- 光标一直指向同一个敌人, Do nothing
	 */

	if (LastActor == nullptr) // LastActor is null
	{
		if (ThisActor != nullptr)
		{
			// Case B
			ThisActor->HightlightActor();
		}
		else
		{
			// Case A - do nothing
		}
	}
	else // LastActor is Valid
	{
		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHightlightActor();
		}
		else // Both Actors are Valid
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHightlightActor();
				ThisActor->HightlightActor();
			}
			else
			{
				// Case E - do nothing
			}
		}
	}


}

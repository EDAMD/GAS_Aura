// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldwon: </><Cooldown>%.1f</>\n\n"


			"<Default>Launches a bolt of fire, exploding on impact and dealing: </>"
			// ScaledDamage
			"<Damage>%d</>"
			"<Default> fire damge with a chance to burn</>\n\n"
			),

			// Values
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage
		);
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldwon: </><Cooldown>%.1f</>\n\n"

			// NumProjectiles
			"<Default>Launches %d bolts of fire, exploding on impact and dealing: </>"
			// ScaledDamage
			"<Damage>%d</>"
			"<Default> fire damge with a chance to burn</>\n\n"
		),

			// Values 
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles), 
			ScaledDamage
		);

	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		// Title
		"<Title>NEXT LEVEL</>\n\n"

		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldwon: </><Cooldown>%.1f</>\n\n"

		// NumProjectiles
		"<Default>Launches %d bolts of fire, exploding on impact and dealing: </>"
		// ScaledDamage
		"<Damage>%d</>"
		"<Default> fire damge with a chance to burn</>\n\n"
	),

		// Values 
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, NumProjectiles),
		ScaledDamage
	);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	// 判断是否在服务器, 只在服务器生成
	const bool bIsServet = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServet)
	{
		return;
	}

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	// 设置俯仰角
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	//NumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());

	const FVector Forward = Rotation.Vector();
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread / 2.f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread / 2.f, FVector::UpVector);

	if (NumProjectiles > 1)
	{
		const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);
		// 分散发射算法
		for (int32 i = 0; i < NumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			const FVector Start = SocketLocation + FVector(0, 0, 20);
			UKismetSystemLibrary::DrawDebugArrow(
				GetAvatarActorFromActorInfo(), 
				Start,
				Start + Direction * 100.f,
				1.f, 
				FLinearColor::Red, 
				120.f, 
				1);
		}
	}
	else
	{
		// 发射单个
		const FVector Start = SocketLocation + FVector(0, 0, 20);
		UKismetSystemLibrary::DrawDebugArrow(
			GetAvatarActorFromActorInfo(),
			Start,
			Start + Forward * 100.f,
			1.f,
			FLinearColor::Red,
			120.f,
			1);
	}
	
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Rotation.Vector() * 100.f, 1.f, FLinearColor::Green, 120.f, 1);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + LeftOfSpread * 100.f, 1.f, FLinearColor::Gray, 120.f, 1);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + RightOfSpread * 100.f, 1.f, FLinearColor::Gray, 120.f, 1);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBlast.h"
#include "Actor/AuraFireBall.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		// Title
		"<Title>FIRE BLAST</>\n\n"

		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldwon: </><Cooldown>%.1f</>\n\n"

		// Commont
		"<Default>Launches</>"
		"Small> %d </>"
		"<Default>fire balls in all directions, each coming back and </>"
		"<Default>exploding upon return, causing</>"

		// ScaledDamage
		"<Damage> %d </>"
		"<Default>radial fire damge with a chance to burn</>\n\n"
	),

		// Values
		Level,
		ManaCost,
		Cooldown,
		NumFireBalls,
		ScaledDamage
	);

}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
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

		// Num
		"<Default>Launches</>"
		"Small> %d </>"
		"<Default>fire balls in all directions, each coming back and </>"
		"<Default>exploding upon return, causing</>"

		// ScaledDamage
		"<Damage> %d </>"
		"<Default>radial fire damge with a chance to burn</>\n\n"
	),

		// Values
		Level,
		ManaCost,
		Cooldown,
		NumFireBalls,
		ScaledDamage
	);
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	TArray<AAuraFireBall*> FireBalls;

	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireBalls);


	for (const FRotator& Rotator : Rotations)
	{
		FTransform Transform;
		Transform.SetLocation(Location);
		Transform.SetRotation(Rotator.Quaternion());
		AAuraFireBall* FireBall =  GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass,
			Transform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FireBall->SetOwner(GetAvatarActorFromActorInfo());
		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();

		FireBall->ExplosionDamageParams = MakeDamageEffectParamsFromClassDefaults();

		FireBalls.Add(FireBall);

		FireBall->FinishSpawning(Transform);
	}




	return FireBalls;
}

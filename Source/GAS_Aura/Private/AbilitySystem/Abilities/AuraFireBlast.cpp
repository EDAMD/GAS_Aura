// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBlast.h"

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

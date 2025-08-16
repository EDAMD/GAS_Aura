// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>ARCANE SHARDS</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldwon: </><Cooldown>%.1f</>\n\n"


			"<Default>summon a shard of arcane energy, causing radial arcane damage of: </>"
			// ScaledDamage
			"<Damage>%d</>"
			"<Default> at the shard origin.</>\n\n"
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
			"<Title>ARCANE SHARDS</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldwon: </><Cooldown>%.1f</>\n\n"

			// Additional Number of Shock Targets
			"<Default>Summon %d shards of arcane energy, causing radial arcane damage of: </>"
			// ScaledDamage
			"<Damage>%d</>"
			"<Default> at the shard origin.</>\n\n"
		),

			// Values 
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumShards),
			ScaledDamage
		);

	}
}

FString UArcaneShards::GetNextLevelDescription(int32 Level)
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

		// Additional Number of Shock Targets
		"<Default>Summon %d shards of arcane energy, causing radial arcane damage of: </>"
		// ScaledDamage
		"<Damage>%d</>"
		"<Default> at the shard origin.</>\n\n"
	),

		// Values 
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxNumShards),
		ScaledDamage
	);
}

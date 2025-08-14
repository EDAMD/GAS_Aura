// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraPassiveAbility.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraPassiveAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAvatarActorFromActorInfo()))
	{
		AuraASC->DeactivatePassiveAbility.AddUObject(this, &UAuraPassiveAbility::RceiveDeactivate);
	}
}

void UAuraPassiveAbility::RceiveDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

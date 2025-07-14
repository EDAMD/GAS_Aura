

#include "Actor/AuraEffectActor.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{

	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Scene"));
}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

}



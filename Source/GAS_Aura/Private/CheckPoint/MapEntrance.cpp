// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint/MapEntrance.h"
#include "Interaction/PlayerInterface.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

void AMapEntrance::HighlightActor_Implementation()
{
	CheckPointMesh->SetRenderCustomDepth(true);
}

void AMapEntrance::LoadActor_Implementation()
{

}

AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	Sphere->SetupAttachment(MoveToComponent);
}

void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;

		if (AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			// Save the world we are traveling to.
			AuraGM->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);

		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint/CheckPoint.h" 
#include "Components/SphereComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Game/AuraGameModeBase.h"



ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{  
    PrimaryActorTick.bCanEverTick = false;  

	CheckPointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckPointMesh");
	CheckPointMesh->SetupAttachment(GetRootComponent());
	CheckPointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckPointMesh->SetCollisionResponseToAllChannels(ECR_Block);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(CheckPointMesh);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CheckPointMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_TAN);
	CheckPointMesh->MarkRenderStateDirty();

	MoveToComponent = CreateDefaultSubobject<USceneComponent>("MoveToComponent");
	MoveToComponent->SetupAttachment(GetRootComponent());
}

void ACheckPoint::HighlightActor_Implementation()
{
	CheckPointMesh->SetRenderCustomDepth(true);
}

void ACheckPoint::UnHighlightActor_Implementation()
{
	CheckPointMesh->SetRenderCustomDepth(false);
}

void ACheckPoint::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	OutDestination = MoveToComponent->GetComponentLocation();
}

void ACheckPoint::LoadActor_Implementation()
{
	if (bReached)
	{
		HandleGlowEffects();
	}
}

void ACheckPoint::BeginPlay()
{
    Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnSphereOverlap);
}

void ACheckPoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;

		if (AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			AuraGM->SaveWorldState(GetWorld());
		}	

		IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
		HandleGlowEffects();
	}
}

void ACheckPoint::HandleGlowEffects()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CheckPointMesh->GetMaterial(0), this);
	CheckPointMesh->SetMaterial(0, DynamicMaterialInstance);
	CheckPointReached(DynamicMaterialInstance);
}

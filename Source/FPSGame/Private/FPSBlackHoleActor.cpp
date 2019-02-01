// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHoleActor.h"
#include <Components/StaticMeshComponent.h>
#include <Components/SphereComponent.h>
#include <Components/PrimitiveComponent.h>
#include <Set.h>
#include <Array.h>

// Sets default values
AFPSBlackHoleActor::AFPSBlackHoleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	EventHorizonComp = CreateDefaultSubobject<USphereComponent>(TEXT("EventHorizonComp"));
	EventHorizonComp->SetSphereRadius(3000);
	EventHorizonComp->SetupAttachment(MeshComp);

	SingularityComp = CreateDefaultSubobject<USphereComponent>(TEXT("SingularityComp"));
	SingularityComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHoleActor::ReachSingularity);
	SingularityComp->SetSphereRadius(100);
	SingularityComp->SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void AFPSBlackHoleActor::BeginPlay()
{
	Super::BeginPlay();
}

void AFPSBlackHoleActor::ReachSingularity(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor) {
		OtherActor->Destroy();
	}
}

// Called every frame
void AFPSBlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TArray<UPrimitiveComponent*> overlappingComponents;

	EventHorizonComp->GetOverlappingComponents(overlappingComponents);
	
	for (auto comp : overlappingComponents) {
		if (comp && comp->IsSimulatingPhysics()) {
			const float Radius = EventHorizonComp->GetScaledSphereRadius();
			const float Strength = -5000;
	
			comp->AddRadialForce(GetActorLocation(), Radius, Strength, ERadialImpulseFalloff::RIF_Constant, true);

		}

	}
}


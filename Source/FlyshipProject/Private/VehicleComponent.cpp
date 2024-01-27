// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleComponent.h"

// Sets default values for this component's properties
UVehicleComponent::UVehicleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVehicleComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* Owner = GetOwner();
	if(Owner)
	Mesh = Cast<UMeshComponent>(Owner->GetRootComponent());
	for (auto Wheel : Wheels)
	{
		UWheel* NewWheel = Cast <UWheel>(Owner->AddComponentByClass(Wheel.WheelClass, false, FTransform(), false));
		if (NewWheel)
		{
			WheelsRef.Add(NewWheel);
			NewWheel->SetParam(Owner, Mesh, Wheel.BoneName, bShowRays);
			FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
			NewWheel->AttachToComponent(Mesh, AttachmentTransformRules, Wheel.BoneName);
		}
	}
	// ...

}


// Called every frame
void UVehicleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


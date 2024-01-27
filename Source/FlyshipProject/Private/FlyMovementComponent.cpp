// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyMovementComponent.h"
#include "Net/UnrealNetwork.h"

UFlyMovementComponent::UFlyMovementComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
}

void UFlyMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}
FBiVector UFlyMovementComponent::CalculateAerodynamicForces(FVector velocity, FVector angularVelocity, FVector wind, float airDensity, FVector centerOfMass)
{

	FBiVector ForceAndTorque(FVector(0.f, 0.f, 0.f), FVector(0.f, 0.f, 0.f));
	if (Surfaces.Num() > 0)
		for (auto Surface : Surfaces)
		{
			if (!Surface) continue;
			FVector RelativePosition = Surface->GetComponentLocation() - centerOfMass;
			ForceAndTorque += Surface->CalculateForces(-velocity + wind - FVector::CrossProduct(angularVelocity, RelativePosition), 1.2f, RelativePosition);

		}
	return ForceAndTorque;
}
void UFlyMovementComponent::Thrust(float axis_value)
{
	if (Owner)
	{
		ThrustAxis = axis_value;
	}
}
void UFlyMovementComponent::CalculateThrustSpeed()
{
	ThrustSpeed = FMath::Clamp(ThrustSpeed + DeltaTimeSeconds * ((ThrustAxis != 0) ? ThrustAcceleration * ThrustAxis : ThrustDeceleration), 0.f, MaxThrustSpeed);
}
void UFlyMovementComponent::ApplyThrustForce()
{
	Mesh->AddForce(Mesh->GetForwardVector() * ThrustSpeed, "", true);
}
void UFlyMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DeltaTimeSeconds = DeltaTime;
	if (!Owner)
	{
		Owner = GetOwner();
		Controller = Owner->GetInstigatorController();
		Mesh = Cast<UMeshComponent>(Owner->GetRootComponent());
			for (auto Children : Owner->GetComponents())
			{
				auto SurfaceWind = Cast<USurfaceWind>(Children);
				if (SurfaceWind)
				{
					Surfaces.Add(SurfaceWind);
					SurfaceWind->SetParam(Mesh, 1.2f);
				}
			}
	}
	if (Mesh->IsSimulatingPhysics())
	{
		CalculateThrustSpeed();
		ApplyThrustForce();
	
	}

}
void UFlyMovementComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
}



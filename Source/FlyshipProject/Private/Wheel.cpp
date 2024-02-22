// Fill out your copyright notice in the Description page of Project Settings.


#include "Wheel.h"

// Sets default values for this component's properties
UWheel::UWheel()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

void UWheel::SetParam(AActor* owner, UMeshComponent* mesh, FName bonename, bool bshowrays)
{
	Owner = owner;
	Mesh = mesh;
	BoneName = bonename;
	bShowRays = bshowrays;
}


// Called when the game starts
void UWheel::BeginPlay()
{
	Super::BeginPlay();
	SetupRayCast();

}

FHitResult UWheel::RayCast(UMeshComponent* mesh, FName bonename, float raylength, AActor* owner)
{
	FHitResult HitResult;
	FVector StartLocation = mesh->GetSocketLocation(bonename);
	FVector EndLocation = StartLocation - UKismetMathLibrary::GetUpVector(mesh->GetSocketRotation(bonename)) * raylength;
	UWorld* World = GetWorld();
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(owner);
	World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, TraceParams);
	return HitResult;
}

void UWheel::SetupRayCast()
{
	PrevLength = SuspensionRestLength;
	RayLength = SuspnesionTravel + SuspensionRestLength + WheelRadius;
}

void UWheel::ResetValues()
{
	Length = SuspensionRestLength + SuspnesionTravel;
	PrevLength = Length;
}

void UWheel::UpdateSuspansionLength()
{
	Length = FMath::Clamp((GetComponentLocation() - (GetUpVector() * WheelRadius + Hit.Location)).Size(), SuspensionRestLength - SuspnesionTravel, SuspensionRestLength + SuspnesionTravel);

}

void UWheel::UpdateSuspansionForce()
{
	float DamperForce = SuspnesionDamper * ((PrevLength - Length) / DeltaTimeSeconds);
	SpringForce = FMath::Clamp((SuspensionRestLength - Length) * SuspnesionStiffnes + DamperForce, ForceMin, ForceMax);
	PrevLength = Length;
}

void UWheel::ApplySuspnesionForce()
{
	FVector Normal = Hit.Normal;
	Normal.Normalize();
	FVector centerOfMass = Mesh->GetCenterOfMass();
	FVector RelativePosition = GetComponentLocation() - centerOfMass;
	if (Mesh->IsSimulatingPhysics())
		Mesh->AddForceAtLocation(Normal * SpringForce * 100, GetComponentLocation());
}

FVector UWheel::GetWheelLinearVelocity()
{
	return UKismetMathLibrary::TransformDirection(GetComponentTransform().Inverse(), Mesh->GetPhysicsLinearVelocityAtPoint(GetComponentLocation() - GetUpVector() * Length) / 100);
}

FVector UWheel::GetTireForce()
{
	FVector tireForce;
	tireForce.X = 0;
	tireForce.Y = FMath::Max(SpringForce, 0.f) * FMath::Clamp(WheelLinearVelocity.Y/-1.f,-1.f,1.f);;
	return tireForce;
}


// Called every frame
void UWheel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DeltaTimeSeconds = DeltaTime;
	if (Owner)
	{
		Hit = RayCast(Mesh, BoneName, RayLength, Owner);
		if (Hit.bBlockingHit)
		{
			UpdateSuspansionLength();
			UpdateSuspansionForce();
			ApplySuspnesionForce();
			WheelLinearVelocity = GetWheelLinearVelocity();
			TireForce = GetTireForce();
			FVector projectRightVector = UKismetMathLibrary::ProjectVectorOnToPlane(GetRightVector(), Hit.Normal);
			projectRightVector.Normalize();
			if(Mesh->IsSimulatingPhysics())
			Mesh->AddForceAtLocation(projectRightVector * TireForce.Y * 100, GetComponentLocation() - GetUpVector() * Length);
	
		}
		else
		{
			ResetValues();
		}
	}

}



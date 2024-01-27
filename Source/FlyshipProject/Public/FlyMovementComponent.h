// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SurfaceWind.h"
#include "BiVector.h"
#include "Engine/ActorChannel.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "FlyMovementComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FLYSHIPPROJECT_API UFlyMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFlyMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
		FBiVector CalculateAerodynamicForces(FVector velocity, FVector angularVelocity, FVector wind, float airDensity, FVector centerOfMass);
	void Thrust(float axis_value);
	void CalculateThrustSpeed();
	void ApplyThrustForce();
protected:
	AActor* Owner;
	UMeshComponent* Mesh;
	float DeltaTimeSeconds = 0.f;
	AController* Controller;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		TArray<USurfaceWind*> Surfaces;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Replicated)
		float ThrustSpeed = 0.f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float ThrustAxis;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float MaxThrustSpeed = 800.f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float ThrustAcceleration = 20.f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float ThrustDeceleration = -10.f;
};

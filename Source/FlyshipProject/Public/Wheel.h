// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Wheel.generated.h"


UCLASS(Blueprintable)
class FLYSHIPPROJECT_API UWheel : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWheel();
	void SetParam(AActor* owner, UMeshComponent* mesh, FName bonename, bool bshowrays);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	FHitResult RayCast(UMeshComponent* mesh, FName bonename, float raylength, AActor* owner);
	void SetupRayCast();
	void ResetValues();
	void UpdateSuspansionLength();
	void UpdateSuspansionForce();
	void ApplySuspnesionForce();
	FVector GetWheelLinearVelocity();
	FVector GetTireForce();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	AActor* Owner = nullptr;
	UMeshComponent* Mesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoneName = "";
	FHitResult Hit;
	bool bShowRays = true;
	float DeltaTimeSeconds = 0.f;
	float SpringForce = 0.f;
	float PrevLength = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		float ForceMin = -1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ForceMax = 6000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Length = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RayLength = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SuspnesionTravel = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SuspensionRestLength = 60.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WheelRadius = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SuspnesionDamper = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SuspnesionStiffnes = 500.f;
	UPROPERTY(BlueprintReadWrite)
		FVector WheelLinearVelocity;
	UPROPERTY(BlueprintReadWrite)
		FVector TireForce;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "FlyMovementComponent.h"
#include "VehicleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Aircraft.generated.h"

UCLASS()
class FLYSHIPPROJECT_API AAircraft : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAircraft();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Thrust(float axis_value);
	void MoveUp(float axis_value);
	void MoveRoll(float axis_value);
	void MoveRight(float axis_value);
	void LookUp(float axis_value);
	void LookLR(float axis_value);
	void ZoomCamera(float axis_value);
	void FlapAction();
	void WheelsAction();
	void ToogleWheels();
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Camera")
	void CameraAction();
	void ToogleCamera();
	void UpdateFlaps(float yaw_axis_value, float roll_axis_value, float pitch_axis_value, float flap_value);
	void FreeCameraAction();
	void ToogleFreeCamera();
public:
	UPROPERTY(BlueprintReadOnly)
		float DeltaTimeSeconds = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* FirstPersonCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* ThirdPersonCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFlyMovementComponent* FlyMovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UVehicleComponent* VehicleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpringArmComponent* FirstPersonSpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpringArmComponent* ThirdPersonSpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float YawAxisValue = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PitchAxisValue = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RollAxisValue = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FlapValue = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PitchAxisSensitivity = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float YawAxisSensitivity = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RollAxisSensitivity = 1.f;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		bool bActiveWheels = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bFreeCamera = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ZoomCameraSpeed = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxCameraArmLength = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MinCameraArmLength = 300.f;
	bool bActiveRollAxis = true;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BiVector.h"
#include "SurfaceWind.generated.h"
USTRUCT(BlueprintType)
struct FConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float LiftSlope;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AspectRatio;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float FlapFraction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ZeroLiftAoA;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float StallAngleHigh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float StallAngleLow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float	Chord = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float	Span = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float	SkinFriction;
};
UENUM(BlueprintType)
enum EInputType
{
	Yaw,
	Pitch,
	Roll,
	Flap,

};

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FLYSHIPPROJECT_API USurfaceWind : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	USurfaceWind();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetParam(UMeshComponent* _mesh, float _airDensity);
	FBiVector CalculateForces(FVector worldAirVelocity, float airDensity, FVector relativePosition);
	float FlapEffectivnessCorrection(float flapAngle);
	float LiftCoefficientMaxFraction(float flapFraction);
	FVector CalculateCoefficients(float angleOfAttack,
		float correctedLiftSlope,
		float zeroLiftAoA,
		float stallAngleHigh,
		float stallAngleLow);
	FVector CalculateCoefficientsAtLowAoA(float angleOfAttack,
		float correctedLiftSlope,
		float zeroLiftAoA);
	float TorqCoefficientProportion(float effectiveAngle);
	FVector CalculateCoefficientsAtStall(float angleOfAttack,
		float correctedLiftSlope,
		float zeroLiftAoA,
		float stallAngleHigh,
		float stallAngleLow);
	float FrictionAt90Degrees(float flapAngle);
	UFUNCTION(BlueprintCallable)
		void SetFlapAngle(float angle);
	UFUNCTION(BlueprintCallable)
		FBiVector CalculateForceAndTorque();
protected:
	UPROPERTY(BlueprintReadWrite)
		UMeshComponent* Mesh;
	float AirDensity = 1.2f;
	const float PREDICTION_TIMESTEP_FRACTION = 0.5f;
	UPROPERTY(BlueprintReadWrite)
		AActor* Owner;
	UPROPERTY(BlueprintReadWrite)
		AController* Controller;
	float AngleAxis;
public:
	UPROPERTY(BlueprintReadWrite)
		float FlapAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FConfig Config;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EInputType> InputType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsControlSurface;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InputMultiplyer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool ShowDEBUG;

};


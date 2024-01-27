// Fill out your copyright notice in the Description page of Project Settings.


#include "SurfaceWind.h"
#include "BiVector.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Engine/ActorChannel.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"

// Sets default values
USurfaceWind::USurfaceWind()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}
// Called when the game starts or when spawned
void USurfaceWind::BeginPlay()
{
	Super::BeginPlay();

}
void USurfaceWind::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(USurfaceWind, FlapAngle);
}
void USurfaceWind::ServerUpdateState_Implementation(float angleAxis)
{
	SetFlapAngle(angleAxis);
}
// Called every frame
void USurfaceWind::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!Owner)
	{
		Owner = GetOwner();
		Controller = Owner->GetInstigatorController();
	}
	if(Controller && Controller->IsLocalController())
	if (Mesh && Mesh->IsSimulatingPhysics())
	{

		if (Owner->HasAuthority())
		{
			//FBiVector forceAndTorqueThisFrame = CalculateForceAndTorque();
		    ///Mesh->AddForce(forceAndTorqueThisFrame.p, "", true);
			//Mesh->AddTorqueInDegrees(forceAndTorqueThisFrame.q * 0.01f, "", true);
		}
	}

}
void USurfaceWind::SetParam(UMeshComponent* _mesh, float _airDensity)
{
	Mesh = _mesh;
	AirDensity = _airDensity;
}
void USurfaceWind::SetFlapAngle(float angleAxis)
{
	if(GetOwner() && !GetOwner()->HasAuthority())
	ServerUpdateState(angleAxis);
	//AngleAxis = angleAxis;
	FlapAngle = FMath::Clamp(angleAxis, -FMath::DegreesToRadians<float>(50), FMath::DegreesToRadians<float>(50));
}
FBiVector USurfaceWind::CalculateForceAndTorque()
{
	FBiVector forceAndTorqueThisFrame(FVector(0.f, 0.f, 0.f), FVector(0.f, 0.f, 0.f));
	FVector velocity = Mesh->GetComponentVelocity();
	FVector wind = FVector(0.f, 0.f, 0.f);
	FVector angularVelocity = Mesh->GetPhysicsAngularVelocityInRadians();
	FVector centerOfMass = Mesh->GetCenterOfMass();
	FVector RelativePosition = GetComponentLocation() - centerOfMass;
	forceAndTorqueThisFrame += CalculateForces(-velocity + wind - FVector::CrossProduct(angularVelocity, RelativePosition), AirDensity, RelativePosition);
	return forceAndTorqueThisFrame;
}
float USurfaceWind::FlapEffectivnessCorrection(float flapAngle)
{
	return FMath::Lerp(0.8f, 0.4f, (FMath::RadiansToDegrees(FMath::Abs(flapAngle)) - 10) / 50);
}

float USurfaceWind::LiftCoefficientMaxFraction(float flapFraction)
{
	return FMath::Clamp((1 - 0.5f * (flapFraction - 0.1f) / 0.3f), 0.0f, 1.0f);
}
FVector USurfaceWind::CalculateCoefficients(float angleOfAttack,
	float correctedLiftSlope,
	float zeroLiftAoA,
	float stallAngleHigh,
	float stallAngleLow)
{
	FVector aerodynamicCoefficients;
	float paddingAngleHigh = FMath::DegreesToRadians(FMath::Lerp(15, 5, (FMath::RadiansToDegrees(FlapAngle) + 50) / 100));
	float paddingAngleLow = FMath::DegreesToRadians(FMath::Lerp(15, 5, (-FMath::RadiansToDegrees(FlapAngle) + 50) / 100));
	float paddedStallAngleHigh = stallAngleHigh + paddingAngleHigh;
	float paddedStallAngleLow = stallAngleLow - paddingAngleLow;

	if (angleOfAttack < stallAngleHigh && angleOfAttack > stallAngleLow)
	{
		// Low angle of attack mode.
		aerodynamicCoefficients = CalculateCoefficientsAtLowAoA(angleOfAttack, correctedLiftSlope, zeroLiftAoA);
	}
	else
	{
		if (angleOfAttack > paddedStallAngleHigh || angleOfAttack < paddedStallAngleLow)
		{
			// Stall mode.
			aerodynamicCoefficients = CalculateCoefficientsAtStall(
				angleOfAttack, correctedLiftSlope, zeroLiftAoA, stallAngleHigh, stallAngleLow);
		}
		else
		{
			// Linear stitching in-between stall and low angles of attack modes.
			FVector aerodynamicCoefficientsLow;
			FVector aerodynamicCoefficientsStall;
			float lerpParam;

			if (angleOfAttack > stallAngleHigh)
			{
				aerodynamicCoefficientsLow = CalculateCoefficientsAtLowAoA(stallAngleHigh, correctedLiftSlope, zeroLiftAoA);
				aerodynamicCoefficientsStall = CalculateCoefficientsAtStall(
					paddedStallAngleHigh, correctedLiftSlope, zeroLiftAoA, stallAngleHigh, stallAngleLow);
				lerpParam = (angleOfAttack - stallAngleHigh) / (paddedStallAngleHigh - stallAngleHigh);
			}
			else
			{
				aerodynamicCoefficientsLow = CalculateCoefficientsAtLowAoA(stallAngleLow, correctedLiftSlope, zeroLiftAoA);
				aerodynamicCoefficientsStall = CalculateCoefficientsAtStall(
					paddedStallAngleLow, correctedLiftSlope, zeroLiftAoA, stallAngleHigh, stallAngleLow);
				lerpParam = (angleOfAttack - stallAngleLow) / (paddedStallAngleLow - stallAngleLow);
			}
			aerodynamicCoefficients = FMath::Lerp(aerodynamicCoefficientsLow, aerodynamicCoefficientsStall, lerpParam);
		}
	}
	return aerodynamicCoefficients;
}
FBiVector USurfaceWind::CalculateForces(FVector worldAirVelocity, float airDensity, FVector relativePosition)
{

	FBiVector ForceAndTorque;
	float correctedLiftSlope = Config.LiftSlope * Config.AspectRatio /
		(Config.AspectRatio + 2 * (Config.AspectRatio + 4) / (Config.AspectRatio + 2));
	float theta = FMath::Acos(2 * Config.FlapFraction - 1);
	float flapEffectivness = 1 - (theta - FMath::Sin(theta)) / PI;
	float deltaLift = correctedLiftSlope * flapEffectivness * FlapEffectivnessCorrection(FlapAngle) * FlapAngle;
	float zeroLiftAoaBase = FMath::DegreesToRadians<float>(Config.ZeroLiftAoA);
	float zeroLiftAoA = zeroLiftAoaBase - deltaLift / correctedLiftSlope;
	float stallAngleHighBase = FMath::DegreesToRadians<float>(Config.StallAngleHigh);
	float stallAngleLowBase = FMath::DegreesToRadians<float>(Config.StallAngleLow);

	float clMaxHigh = correctedLiftSlope * (stallAngleHighBase - zeroLiftAoaBase) + deltaLift * LiftCoefficientMaxFraction(Config.FlapFraction);
	float clMaxLow = correctedLiftSlope * (stallAngleLowBase - zeroLiftAoaBase) + deltaLift * LiftCoefficientMaxFraction(Config.FlapFraction);

	float stallAngleHigh = zeroLiftAoA + clMaxHigh / correctedLiftSlope;
	float stallAngleLow = zeroLiftAoA + clMaxLow / correctedLiftSlope;

	FVector airVelocity = UKismetMathLibrary::InverseTransformDirection(GetComponentTransform(), worldAirVelocity);
	airVelocity = FVector(airVelocity.X, 0, airVelocity.Z);
	FVector NormalazizeAirVelocity = airVelocity;
	NormalazizeAirVelocity.Normalize();
	FVector dragDirection = UKismetMathLibrary::TransformDirection(GetComponentTransform(), NormalazizeAirVelocity) / 100;
	dragDirection.Normalize();
	FVector NormalizeForwardVector = GetForwardVector() / 100;
	NormalizeForwardVector.Normalize();
	FVector NormalizeUpVector = GetUpVector() / 100;
	NormalizeUpVector.Normalize();
	FVector NormalizeRightVector = GetRightVector() / 100;
	NormalizeRightVector.Normalize();
	FVector liftDirection = FVector::CrossProduct(dragDirection, -NormalizeRightVector);
	float area = Config.Chord * Config.Span;
	float dynamicPressure = 0.5f * airDensity * (airVelocity / 100).SizeSquared();
	float angleOfAttack = FMath::Atan2(airVelocity.Z / 100, -airVelocity.X / 100);
	FVector aerodynamicCoefficients = CalculateCoefficients(angleOfAttack,
		correctedLiftSlope,
		zeroLiftAoA,
		stallAngleHigh,
		stallAngleLow);
	FVector lift = liftDirection * aerodynamicCoefficients.X * dynamicPressure * area;
	FVector drag = dragDirection * aerodynamicCoefficients.Y * dynamicPressure * area;
	FVector torque = -NormalizeRightVector * aerodynamicCoefficients.Z * dynamicPressure * area * Config.Chord;
	ForceAndTorque.p += lift + drag;
	ForceAndTorque.q += FVector::CrossProduct(relativePosition / 100, ForceAndTorque.p);
	ForceAndTorque.q += torque;
	// DEBUG ARROWS
	if (ShowDEBUG)
	{
		DrawDebugDirectionalArrow(GetWorld(), GetComponentLocation(), ForceAndTorque.p + GetComponentLocation(), 1.f, FColor::Yellow, false, 0, 0, 1.f);
		DrawDebugDirectionalArrow(GetWorld(), GetComponentLocation(), dragDirection * 100 + GetComponentLocation(), 1.f, FColor::Red, false, 0, 0, 1.f);
		DrawDebugDirectionalArrow(GetWorld(), GetComponentLocation(), liftDirection * 100 + GetComponentLocation(), 1.f, FColor::Blue, false, 0, 0, 1.f);
		DrawDebugBox(GetWorld(), GetComponentLocation(), FVector(Config.Chord * 100, Config.Span * 100, 0), GetComponentRotation().Quaternion(), FColor::Purple, false, 0, 0, 1);
	}
	return ForceAndTorque;

}
FVector USurfaceWind::CalculateCoefficientsAtLowAoA(float angleOfAttack,
	float correctedLiftSlope,
	float zeroLiftAoA)
{
	float liftCoefficient = correctedLiftSlope * (angleOfAttack - zeroLiftAoA);
	float inducedAngle = liftCoefficient / (PI * Config.AspectRatio);
	float effectiveAngle = angleOfAttack - zeroLiftAoA - inducedAngle;

	float tangentialCoefficient = Config.SkinFriction * FMath::Cos(effectiveAngle);

	float normalCoefficient = (liftCoefficient +
		FMath::Sin(effectiveAngle) * tangentialCoefficient) / FMath::Cos(effectiveAngle);
	float dragCoefficient = normalCoefficient * FMath::Sin(effectiveAngle) + tangentialCoefficient * FMath::Cos(effectiveAngle);
	float torqueCoefficient = -normalCoefficient * TorqCoefficientProportion(effectiveAngle);

	return FVector(liftCoefficient, dragCoefficient, torqueCoefficient);
}

float USurfaceWind::TorqCoefficientProportion(float effectiveAngle)
{
	return 0.25f - 0.175f * (1 - 2 * FMath::Abs(effectiveAngle) / PI);
}

FVector USurfaceWind::CalculateCoefficientsAtStall(float angleOfAttack,
	float correctedLiftSlope,
	float zeroLiftAoA,
	float stallAngleHigh,
	float stallAngleLow)
{
	float liftCoefficientLowAoA;
	if (angleOfAttack > stallAngleHigh)
	{
		liftCoefficientLowAoA = correctedLiftSlope * (stallAngleHigh - zeroLiftAoA);
	}
	else
	{
		liftCoefficientLowAoA = correctedLiftSlope * (stallAngleLow - zeroLiftAoA);
	}
	float inducedAngle = liftCoefficientLowAoA / (PI * Config.AspectRatio);

	float lerpParam;
	if (angleOfAttack > stallAngleHigh)
	{
		lerpParam = (PI / 2 - FMath::Clamp(angleOfAttack, -PI / 2, PI / 2))
			/ (PI / 2 - stallAngleHigh);
	}
	else
	{
		lerpParam = (-PI / 2 - FMath::Clamp(angleOfAttack, -PI / 2, PI / 2))
			/ (-PI / 2 - stallAngleLow);
	}
	inducedAngle = FMath::Lerp(0.0f, inducedAngle, lerpParam);
	float effectiveAngle = angleOfAttack - zeroLiftAoA - inducedAngle;

	float normalCoefficient = FrictionAt90Degrees(FlapAngle) * FMath::Sin(effectiveAngle) *
		(1 / (0.56f + 0.44f * FMath::Abs(FMath::Sin(effectiveAngle))) -
			0.41f * (1 - FMath::Exp(-17 / Config.AspectRatio)));
	float tangentialCoefficient = 0.5f * Config.SkinFriction * FMath::Cos(effectiveAngle);

	float liftCoefficient = normalCoefficient * FMath::Cos(effectiveAngle) - tangentialCoefficient * FMath::Sin(effectiveAngle);
	float dragCoefficient = normalCoefficient * FMath::Sin(effectiveAngle) + tangentialCoefficient * FMath::Cos(effectiveAngle);
	float torqueCoefficient = -normalCoefficient * TorqCoefficientProportion(effectiveAngle);

	return FVector(liftCoefficient, dragCoefficient, torqueCoefficient);
}

float USurfaceWind::FrictionAt90Degrees(float flapAngle)
{
	return 1.98f - 4.26e-2f * flapAngle * flapAngle + 2.1e-1f * flapAngle;
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "Aircraft.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AAircraft::AAircraft()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SetRootComponent(SkeletalMesh);
	FlyMovementComponent = CreateDefaultSubobject<UFlyMovementComponent>(TEXT("FlyMovementComponent"));
	VehicleComponent = CreateDefaultSubobject<UVehicleComponent>(TEXT("VehicleComponent"));
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	FirstPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("FirstPersonSpringArm"));
	ThirdPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonSpringArm"));
	FirstPersonCamera->SetupAttachment(FirstPersonSpringArm);
	ThirdPersonCamera->SetupAttachment(ThirdPersonSpringArm);
	FirstPersonSpringArm->SetupAttachment(SkeletalMesh);
	ThirdPersonSpringArm->SetupAttachment(SkeletalMesh);
	ThirdPersonSpringArm->bInheritPitch = false;
	ThirdPersonSpringArm->bInheritRoll = false;

}
// Called when the game starts or when spawned
void AAircraft::BeginPlay()
{
	Super::BeginPlay();

}
void AAircraft::Thrust(float axis_value)
{
	FlyMovementComponent->Thrust(axis_value);
}

void AAircraft::MoveUp(float axis_value)
{
	PitchAxisValue = PitchAxisSensitivity * axis_value;
}

void AAircraft::MoveRoll(float axis_value)
{
	if (bActiveRollAxis)
		RollAxisValue = RollAxisSensitivity * axis_value;

}

void AAircraft::MoveRight(float axis_value)
{
	YawAxisValue = YawAxisSensitivity * axis_value;
}

void AAircraft::LookUp(float axis_value)
{
	AddControllerPitchInput(axis_value);
}

void AAircraft::LookLR(float axis_value)
{
	AddControllerYawInput(axis_value);
}

void AAircraft::ZoomCamera(float axis_value)
{
	if (ThirdPersonCamera->IsActive())
	{
		ThirdPersonSpringArm->TargetArmLength = FMath::Clamp(axis_value * -1.f * ZoomCameraSpeed + ThirdPersonSpringArm->TargetArmLength, MinCameraArmLength, MaxCameraArmLength);
	}
}

void AAircraft::FlapAction()
{
	FlapValue = (FlapValue == 0) ? 1 : 0;
}
void AAircraft::WheelsAction()
{
	ToogleWheels();
}
void AAircraft::ToogleWheels()
{
	bActiveWheels = !bActiveWheels;
}
void AAircraft::CameraAction_Implementation()
{
	if (bFreeCamera)
		ToogleFreeCamera();
	ToogleCamera();

}
void AAircraft::ToogleCamera()
{
	if (ThirdPersonCamera->IsActive())
	{
		ThirdPersonCamera->Deactivate();
		FirstPersonCamera->Activate();
	}
	else
	{
		FirstPersonCamera->Deactivate();
		ThirdPersonCamera->Activate();
	}
}
void AAircraft::UpdateFlaps(float yaw_axis_value, float roll_axis_value, float pitch_axis_value, float flap_value)
{
	float Angle = 0.f;
	if (FlyMovementComponent->Surfaces.Num() <= 0) return;
		for (auto Surface : FlyMovementComponent->Surfaces)
		{
			if (Surface && Surface->IsControlSurface)
			{
				switch (Surface->InputType)
				{
				case Yaw: Angle = Surface->InputMultiplyer * yaw_axis_value;
					break;
				case Pitch: Angle = Surface->InputMultiplyer * pitch_axis_value;
					break;
				case Roll: Angle = Surface->InputMultiplyer * roll_axis_value;
					break;
				case Flap: Angle = Surface->InputMultiplyer * flap_value;
					break;

				}
				Surface->SetFlapAngle(Angle);
			}
		}
}

void AAircraft::FreeCameraAction()
{
	ToogleFreeCamera();
}

void AAircraft::ToogleFreeCamera()
{
	bFreeCamera = !bFreeCamera;
	if (ThirdPersonCamera->IsActive())
	{
		ThirdPersonSpringArm->bUsePawnControlRotation = bFreeCamera;
		ThirdPersonSpringArm->bInheritPitch = bFreeCamera;
		ThirdPersonSpringArm->bInheritRoll = bFreeCamera;
	}
	else
	{
		FirstPersonSpringArm->bUsePawnControlRotation = bFreeCamera;
	}
	bActiveRollAxis = !bFreeCamera; // Active/Deactive Roll, when use FreeCamera

}

// Called every frame
void AAircraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DeltaTimeSeconds = DeltaTime;
	UpdateFlaps(YawAxisValue, RollAxisValue, PitchAxisValue, FlapValue);
}

// Called to bind functionality to input
void AAircraft::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("LookUp", this, &AAircraft::LookUp);
	PlayerInputComponent->BindAxis("LookLR", this, &AAircraft::LookLR);
	PlayerInputComponent->BindAxis("Thrust", this, &AAircraft::Thrust);
	PlayerInputComponent->BindAxis("MoveUp", this, &AAircraft::MoveUp);
	PlayerInputComponent->BindAxis("MoveRoll", this, &AAircraft::MoveRoll);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAircraft::MoveRight);
	PlayerInputComponent->BindAxis("ZoomCamera", this, &AAircraft::ZoomCamera);
	PlayerInputComponent->BindAction("Flap", IE_Pressed, this, &AAircraft::FlapAction);
	PlayerInputComponent->BindAction("Wheels", IE_Pressed, this, &AAircraft::WheelsAction);
	PlayerInputComponent->BindAction("Camera", IE_Pressed, this, &AAircraft::CameraAction);
	PlayerInputComponent->BindAction("FreeCamera", IE_Pressed, this, &AAircraft::FreeCameraAction);

}
void AAircraft::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
}

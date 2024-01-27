// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Wheel.h"
#include "WheelStruct.h"
#include "VehicleComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FLYSHIPPROJECT_API UVehicleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UVehicleComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UMeshComponent* Mesh;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TArray<FWheelStruct> Wheels;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TArray<UWheel*> WheelsRef;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool bShowRays = true;

};

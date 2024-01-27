// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Wheel.h"
#include "WheelStruct.generated.h"

USTRUCT(BlueprintType)
struct FWheelStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TSubclassOf<UWheel> WheelClass = nullptr;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FName BoneName = "";


};

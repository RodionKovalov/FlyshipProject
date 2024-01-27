// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BiVector.generated.h"

USTRUCT(BlueprintType)
struct FBiVector
{
	GENERATED_BODY()
public:
	FBiVector();
	FBiVector(FVector a, FVector b);
	FBiVector operator+(const FBiVector& a) const;
	FBiVector operator+=(const FBiVector& a);
	FBiVector operator*(float f) const;
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FVector p = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FVector q = FVector(0.0f, 0.0f, 0.0f);
};
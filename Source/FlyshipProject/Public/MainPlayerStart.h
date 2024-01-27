// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Components/CapsuleComponent.h"
#include "MainPlayerStart.generated.h"

/**
 *
 */
UCLASS()
class FLYSHIPPROJECT_API AMainPlayerStart : public APlayerStart
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	bool IsFree();
protected:

	UFUNCTION()

		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	bool bFree = true;
};

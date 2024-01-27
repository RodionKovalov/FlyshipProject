// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainGameComponents.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FLYSHIPPROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void StartRespawnCharacter(float time);
protected:
	FTimerHandle RespawnTimerHandle;
	void RespawnCharacter();
};

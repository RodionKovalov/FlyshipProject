// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

void AMainPlayerController::StartRespawnCharacter(float time)
{
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AMainPlayerController::RespawnCharacter, time, false);
}

void AMainPlayerController::RespawnCharacter()
{
	AMainGameMode* gameMode = nullptr;
	RespawnTimerHandle.Invalidate();
	if (GetWorld())
		gameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode)
		if (!gameMode->SpawnPlayer(this))
		{
			StartRespawnCharacter(gameMode->RespawnTime);
		}

}


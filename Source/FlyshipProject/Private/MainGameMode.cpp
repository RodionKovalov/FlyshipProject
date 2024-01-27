// Fill out your copyright notice in the Description page of Project Settings.

#include "MainGameMode.h"

AMainGameMode::AMainGameMode()
{
	StartWaitTime = 2.f;
	RespawnTime = 2.f;
}
void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		StartSpawnPlayers();
	}

}
void AMainGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (GetWorld())
	{
		AMainGameState* gameState = Cast<AMainGameState>(UGameplayStatics::GetGameState(GetWorld()));
		if (gameState)
		{
			if (gameState->GameHasStarted())
				SpawnPlayer(NewPlayer);
		}
		else UE_LOG(LogTemp, Warning, TEXT("GameState is not created"));

	}
	else
	UE_LOG(LogTemp, Warning, TEXT("World is not created"));
}
void AMainGameMode::DeadCharacter(APawn* character)
{
	AMainPlayerController* playerController = nullptr;
	if (character)
		playerController = Cast<AMainPlayerController>(character->GetController());
	if (playerController)
	{
		ASpectatorPawn* spectator = SpawnSpectator(character->GetTransform());
		if (spectator)
			playerController->Possess(spectator);
		playerController->StartRespawnCharacter(RespawnTime);
		AMainPlayerState* playerState = playerController->GetPlayerState<AMainPlayerState>();
		if (playerState)
		{
			playerState->SetAlive(false);
			playerState->OnRep_Alive();
		}
	}
}
void AMainGameMode::StartSpawnPlayers()
{
	if (GetWorld())
		GetWorld()->GetTimerManager().SetTimer(SpawnPlayersTimerHandle, this, &AMainGameMode::StartGame, StartWaitTime, false);
}
void AMainGameMode::SpawnPlayers()
{
	TArray<APlayerState*> playersState = GetGameState<AGameStateBase>()->PlayerArray;
	for (auto playerState : playersState)
	{
		if (playerState)
		{
			AController* controller = Cast<AController>(playerState->GetOwner());
			if (controller)
				SpawnPlayer(controller);
		}
	}
}
ASpectatorPawn* AMainGameMode::SpawnSpectator(FTransform transform)
{
	ASpectatorPawn* spectator = nullptr;
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (GetWorld())
	{
		spectator = GetWorld()->SpawnActor<ASpectatorPawn>(SpectatorClass, transform, spawnParams);
	}
	return spectator;
}
bool AMainGameMode::SpawnPlayer(AController* controller)
{
	bool successful = false;
	APawn* character = nullptr;
	TArray<AMainPlayerStart*> spawnPoints = GetFreeSpawnPoints();
	if (spawnPoints.Num() > 0)
	{
		character = SpawnCharacter(spawnPoints[0], controller);
		if (character && controller)
		{
			controller->Possess(character);
			AMainPlayerState* playerState = controller->GetPlayerState<AMainPlayerState>();
			if (playerState)
			{
				playerState->SetAlive(true);
				playerState->OnRep_Alive();
			}
			successful = true;
		}
	}
	return successful;

}
APawn* AMainGameMode::SpawnCharacter(AMainPlayerStart* playerstart, AController* controller)
{
	APawn* character = nullptr;
	FVector location = playerstart->GetActorLocation();
	FRotator rotation = playerstart->GetActorRotation();
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (GetWorld())
	{
		character = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, location, rotation, spawnParams);
	}
	return character;


}
void AMainGameMode::StartGame()
{
	SpawnPlayers();
	OnGameStartedDelegate.Broadcast();
}
TArray<AMainPlayerStart*> AMainGameMode::GetFreeSpawnPoints()
{
	TArray<AActor*> points;
	TArray<AMainPlayerStart*> freePoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainPlayerStart::StaticClass(), points);
	check(points.Num() > 0);
	if (points.Num() > 0)
		for (auto point : points)
		{
			AMainPlayerStart* spawnPoint = Cast<AMainPlayerStart>(point);
			if (spawnPoint && spawnPoint->IsFree())
			{
				freePoints.Add(spawnPoint);
			}

		}
	return freePoints;
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "MainGameComponents.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStartedSignature);

class AMainGameState;
class AMainPlayerStart;

UCLASS()
class FLYSHIPPROJECT_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	void DeadCharacter(APawn* character);
	bool SpawnPlayer(AController* controller);
protected:
	FTimerHandle RespawnPlayerTimerHandle;
	FTimerHandle SpawnPlayersTimerHandle;
	void StartSpawnPlayers();
	void SpawnPlayers();
	ASpectatorPawn* SpawnSpectator(FTransform transform);
	APawn* SpawnCharacter(AMainPlayerStart* playerstart, AController* controller);
	UFUNCTION()
		void StartGame();
	TArray<AMainPlayerStart*>  GetFreeSpawnPoints();
public:
	AMainGameMode();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StartWaitTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RespawnTime;
	UPROPERTY(BlueprintAssignable)
		FOnGameStartedSignature OnGameStartedDelegate;

};

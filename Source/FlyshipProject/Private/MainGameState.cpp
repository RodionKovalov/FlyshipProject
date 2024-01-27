// Fill out your copyright notice in the Description page of Project Settings.

#include "MainGameState.h"
#include "Net/UnrealNetwork.h"
void AMainGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMainGameState, bGameStarted);

}
void AMainGameState::OnRep_GameStarted()
{
	if (bGameStarted)
	{
		GameStartedDelegate.Broadcast();
	}
}
void AMainGameState::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode)
			GameMode->OnGameStartedDelegate.AddDynamic(this, &AMainGameState::StartGame);
	}
}
void AMainGameState::StartGame()
{
	bGameStarted = true;
	OnRep_GameStarted();
}
bool AMainGameState::GameHasStarted()
{
	return bGameStarted;
}

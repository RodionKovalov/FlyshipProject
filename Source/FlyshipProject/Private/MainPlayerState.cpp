// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerState.h"
#include "Net/UnrealNetwork.h"

void AMainPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMainPlayerState, bAlive);
}
AMainPlayerState::AMainPlayerState()
{
	bReplicates = true;
}
bool AMainPlayerState::GetAlive()
{
	return bAlive;
}
void AMainPlayerState::SetAlive(bool value)
{
	bAlive = value;
}
void AMainPlayerState::OnRep_Alive()
{
	if (bAlive)
	{
		PlayerAliveDelegate.Broadcast();
	}
	else PlayerDeadDelegate.Broadcast();
}

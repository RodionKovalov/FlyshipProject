// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDeadSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerAliveSignature);
UCLASS()
class FLYSHIPPROJECT_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AMainPlayerState();
	bool GetAlive();
	void SetAlive(bool value);
	UFUNCTION()
		void OnRep_Alive();
protected:
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
public:
	UPROPERTY(BlueprintAssignable)
		FPlayerDeadSignature PlayerDeadDelegate;
	UPROPERTY(BlueprintAssignable)
		FPlayerAliveSignature PlayerAliveDelegate;
protected:
	UPROPERTY(ReplicatedUsing = OnRep_Alive)
		bool bAlive = false;
};

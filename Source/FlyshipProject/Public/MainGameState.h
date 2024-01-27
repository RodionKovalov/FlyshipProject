// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainGameComponents.h"
#include "GameFramework/GameStateBase.h"
#include "MainGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStartedSignature);

class AMainGameMode;

UCLASS()
class FLYSHIPPROJECT_API AMainGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	bool GameHasStarted();
protected:
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	UFUNCTION()
	void OnRep_GameStarted();
	UFUNCTION()
		void StartGame();
public:
	UPROPERTY(BlueprintAssignable)
		FGameStartedSignature GameStartedDelegate;
	UPROPERTY(ReplicatedUsing=OnRep_GameStarted)
		bool bGameStarted = false;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerStart.h"

void AMainPlayerStart::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()-> SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Trigger"), false);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMainPlayerStart::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMainPlayerStart::OnEndOverlap);
}

bool AMainPlayerStart::IsFree()
{
	return bFree;
}

void AMainPlayerStart::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bFree = false;
}
void AMainPlayerStart::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bFree = true;
}
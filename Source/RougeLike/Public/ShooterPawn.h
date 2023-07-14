// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterPawn.generated.h"

UCLASS()
class ROUGELIKE_API AShooterPawn : public AActor
{
	GENERATED_BODY()

		float X, Y;
	

public:
	// Sets default values for this pawn's properties
	AShooterPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	




};

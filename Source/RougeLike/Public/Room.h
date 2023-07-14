// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AICharacter.h"
#include "Room.generated.h"

UCLASS()
class ROUGELIKE_API ARoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/*
	UENUM(BlueprintType, Category = "Room Info")
		enum class RoomID : uint8 {
		ID_NormalRoom      UMETA(DisplayName = "Normal Room"),
		ID_ShopRoom        UMETA(DisplayName = "Shop Room"),
		ID_BossRoom        UMETA(DisplayName = "Boss Room"),
	};
	*/

	UFUNCTION(BlueprintImplementableEvent)
		void SetExitPoints();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Info")
		TArray<UActorComponent*> exitPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Info")
		bool hasEntered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Info")
		int numOfEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Info")
		TArray<TSubclassOf<class AAICharacter>> enemiesToSpawn;

};

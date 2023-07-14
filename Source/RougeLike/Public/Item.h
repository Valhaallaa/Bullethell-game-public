// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item.generated.h"

UCLASS()
class ROUGELIKE_API AItem : public AActor
{
	GENERATED_BODY()


		UFUNCTION()
		virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		virtual void PickupItem();

	virtual void UseItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
		bool isPassive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
		int spawnChance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
		int shopCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
		bool hasSpawnedInShop;
};

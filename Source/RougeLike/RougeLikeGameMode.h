// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GameFramework/GameModeBase.h"
#include "RougeLikeGameMode.generated.h"

UCLASS(minimalapi)
class ARougeLikeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARougeLikeGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	int coinAmount;

	int GetCoinAmount();
	void AddCoins(int value);

	bool roomCleared = false;

	int numOfEnemiesSpawned;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
		AItem* itemHeld;

	UFUNCTION(BlueprintCallable)
		void AllEnemiesKilled();

	void UseHeldItem();

	UFUNCTION(BlueprintCallable)
		void OnRoomClear();

	UFUNCTION(BlueprintImplementableEvent)
		void SaveInfo();

};




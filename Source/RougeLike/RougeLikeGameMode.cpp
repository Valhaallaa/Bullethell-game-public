// Copyright Epic Games, Inc. All Rights Reserved.

#include "RougeLikeGameMode.h"
#include "RougeLikeCharacter.h"
#include "RoomSpawner.h"
#include "AICharacter.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"

ARougeLikeGameMode::ARougeLikeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

int ARougeLikeGameMode::GetCoinAmount()
{
	return coinAmount;
}

void ARougeLikeGameMode::AddCoins(int value)
{
	coinAmount += value;
}

void ARougeLikeGameMode::AllEnemiesKilled()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnKilled"));
	
	if (numOfEnemiesSpawned == 0)
		OnRoomClear();
	/*
	for (TActorIterator<AAICharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (!ActorItr)
		{
			OnRoomClear();
		}
	}*/
}

void ARougeLikeGameMode::UseHeldItem()
{
	if (itemHeld) {
		itemHeld->UseItem();
		itemHeld = nullptr;
	}
}



void ARougeLikeGameMode::OnRoomClear()
{
	roomCleared = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnRoomClear"));

	for (TActorIterator<ARoomSpawner> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr)
		{
			ActorItr->SpawnItem();
		}
	}
}

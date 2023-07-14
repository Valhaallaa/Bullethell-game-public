// Fill out your copyright notice in the Description page of Project Settings.
#include "MoneyJar.h"
#include "RougeLike\RougeLikeGameMode.h"


void AMoneyJar::UseItem()
{
	ARougeLikeGameMode* gameMode = Cast<ARougeLikeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (gameMode) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MONEY JAR"));

		gameMode->AddCoins(coinsToAdd);

	}
	
}

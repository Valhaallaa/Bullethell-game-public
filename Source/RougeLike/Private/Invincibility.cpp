// Fill out your copyright notice in the Description page of Project Settings.

#include "Invincibility.h"
#include "RougeLike\RougeLikeCharacter.h"
#include "RougeLike\RougeLikeGameMode.h"

void AInvincibility::UseItem()
{
	
	ARougeLikeCharacter* player = Cast<ARougeLikeCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (player) {
		player->PlayerHealth->SetTakeDamage(false);

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &AInvincibility::StopInvincibility, 10, false);

	}
	
}

void AInvincibility::StopInvincibility() 
{
	
	ARougeLikeCharacter* player = Cast<ARougeLikeCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	player->PlayerHealth->SetTakeDamage(true);
	ARougeLikeGameMode* gameMode = Cast<ARougeLikeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->itemHeld = nullptr;
}



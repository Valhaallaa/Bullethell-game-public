// Fill out your copyright notice in the Description page of Project Settings.

#include "FireRate.h"
#include "RougeLike\RougeLikeCharacter.h"

void AFireRate::UseItem() 
{
	
	ARougeLikeCharacter* player = Cast<ARougeLikeCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (player)
	{
		player->PlayerWeapon->shootCooldown = player->PlayerWeapon->shootCooldown/2.0f;

		FTimerHandle fireRate;
		GetWorldTimerManager().SetTimer(
			fireRate, this, &AFireRate::DefaultFireRate, 120, false);
	}
	
}

void AFireRate::DefaultFireRate()
{
	
	ARougeLikeCharacter* player = Cast<ARougeLikeCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (player)
	{
		player->PlayerWeapon->shootCooldown = 0.5f;
	}
	
}


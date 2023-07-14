// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeedItem.h"
#include "RougeLike\RougeLikeCharacter.h"


void ASpeedItem::UseItem()
{
	
	ARougeLikeCharacter* player =  Cast<ARougeLikeCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (player) {
		player->GetCharacterMovement()->MaxWalkSpeed += (player->GetCharacterMovement()->MaxWalkSpeed * 0.1);
	}
	
}

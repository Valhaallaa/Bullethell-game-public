// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Invincibility.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKE_API AInvincibility : public AItem
{
	GENERATED_BODY()

		virtual void UseItem() override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
		int coinsToAdd;

	void StopInvincibility();

};

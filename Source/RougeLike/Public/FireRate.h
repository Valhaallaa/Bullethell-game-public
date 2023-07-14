// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "FireRate.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKE_API AFireRate : public AItem
{
	GENERATED_BODY()
		virtual void UseItem() override;

	void DefaultFireRate();
};

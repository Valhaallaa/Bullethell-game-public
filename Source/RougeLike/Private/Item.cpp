// Fill out your copyright notice in the Description page of Project Settings.
#include "Item.h"
#include "RougeLike\RougeLikeGameMode.h"
#include "RougeLike\RougeLikeCharacter.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("overlap"));
	ARougeLikeCharacter* player = Cast<ARougeLikeCharacter>(OtherActor);
	if (player) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("overlap"));

		PickupItem();
	}
	*/
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AItem::PickupItem()
{
	ARougeLikeGameMode* gameMode = Cast<ARougeLikeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (gameMode)
	{
		if (hasSpawnedInShop)
		{
			if (gameMode->coinAmount >= shopCost)
			{
				if (!isPassive)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BUY"));
					gameMode->coinAmount -= shopCost;
					gameMode->itemHeld = this;
					this->Destroy();
				}
				else {
					gameMode->coinAmount -= shopCost;
					UseItem();
					this->Destroy();
				}
			}
		}
		else {

			if (!isPassive)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PICKUP"));
				gameMode->itemHeld = this;
				this->Destroy();
			}
			else {
				UseItem();
				this->Destroy();
			}
		}
	}
}

void AItem::UseItem()
{
}


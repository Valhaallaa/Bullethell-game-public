// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomSpawner.h"
#include "..\Public\RoomSpawner.h"
// Sets default values
ARoomSpawner::ARoomSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARoomSpawner::BeginPlay()
{
    Super::BeginPlay();

    SpawnEnemies();
}


void ARoomSpawner::CalculateNewRoom()
{
	int total = normalRoomPercentage + shopRoomPercentage + bossRoomPercentage;
	int twoBound = normalRoomPercentage + shopRoomPercentage;
	int randNum = FMath::RandRange(0, total);

    if (randNum <= normalRoomPercentage)
    {
        roomNum = 0;
    }
    else if (randNum > normalRoomPercentage && randNum <= twoBound)
    {
        roomNum = 1;
    }
    else
    {
        roomNum = 2;
    }

}

// Called every frame
void ARoomSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ARoomSpawner::SpawnNewRoom(UActorComponent* exit, UStaticMeshComponent* door)
{
    ARougeLikeGameMode* gameMode = Cast<ARougeLikeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (gameMode && gameMode->roomCleared || currRoom->ActorHasTag(FName("Shop")))
    {
        FActorSpawnParameters spawnParams;
        spawnParams.Owner = GetOwner();
        spawnParams.Instigator = nullptr;
        spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(exit);

        int corridorChosenNum = FMath::FRandRange(0, corridors.Num());
        TSubclassOf<class ARoom> corridorChosen = corridors[corridorChosenNum];

        ARoom* corridor = GetWorld()->SpawnActor<ARoom>(corridorChosen, MeshComp->GetComponentLocation(), MeshComp->GetComponentRotation(), spawnParams);
        oldCorridor = newCorridor;
        newCorridor = corridor;
        corridor->SetExitPoints();
        int corridorExit = FMath::FRandRange(0, corridor->exitPoints.Num());
        UActorComponent* corridorExitChosen = corridor->exitPoints[corridorExit];

        UStaticMeshComponent* CorridorMeshComp = Cast<UStaticMeshComponent>(corridorExitChosen);

        int total = normalRoomPercentage + shopRoomPercentage + bossRoomPercentage;
        int twoBound = normalRoomPercentage + shopRoomPercentage;
        int randRoomNum = FMath::RandRange(0, total);


        if (randRoomNum <= normalRoomPercentage) //checks what type of room spawns
        {
            roomNum = 0; //Normal
        }
        else if (randRoomNum > normalRoomPercentage && randRoomNum <= twoBound)
        {
            if (numSinceLastShop < minRoomsBetweenShops)
                roomNum = 0;
            else
            {
                roomNum = 1; //shop
            }
        }
        else
        {
            if (numSinceLastBoss < minRoomsBetweenBoss)
                roomNum = 0;
            else
            {
                roomNum = 2; //shop
            }

        }


        if (numSinceLastBoss == garuanteedBossNum) {
            roomNum = 2;
        }

        if (numSinceLastShop == garuanteedShopNum) {
            roomNum = 1;
        }


        numSinceLastShop++;
        numSinceLastBoss++;

        if (roomNum == 0) //normalRoom
        {
            int roomChosenNum = FMath::FRandRange(0, normalRooms.Num());
            TSubclassOf<class ARoom> roomChosen = normalRooms[roomChosenNum];


            ARoom* roomSpawned = GetWorld()->SpawnActor<ARoom>(roomChosen, CorridorMeshComp->GetComponentLocation(), CorridorMeshComp->GetComponentRotation(), spawnParams); // spawn normal room at end of corridor

            oldRoom = currRoom;
            currRoom = roomSpawned;
        }
        if (roomNum == 1) //Shop
        {
            int roomChosenNum = FMath::FRandRange(0, shopRooms.Num());
            TSubclassOf<class ARoom> roomChosen = shopRooms[roomChosenNum];

            ARoom* shopRoomSpawned = GetWorld()->SpawnActor<ARoom>(roomChosen, CorridorMeshComp->GetComponentLocation(), CorridorMeshComp->GetComponentRotation(), spawnParams);

            oldRoom = currRoom;
            currRoom = shopRoomSpawned;

            SpawnShopItems(shopRoomSpawned);
            // gameMode->AllEnemiesKilled();

            numSinceLastShop = 0;
        }

        if (roomNum == 2) //Boss
        {
            int roomChosenNum = FMath::FRandRange(0, bossRooms.Num());
            TSubclassOf<class ARoom> roomChosen = bossRooms[roomChosenNum];

            ARoom* bossRoomSpawned = GetWorld()->SpawnActor<ARoom>(roomChosen, CorridorMeshComp->GetComponentLocation(), CorridorMeshComp->GetComponentRotation(), spawnParams);

            oldRoom = currRoom;
            currRoom = bossRoomSpawned;

            numSinceLastBoss = 0;
        }
        door->DestroyComponent();
        gameMode->roomCleared = false;
        return true;

    }
    else {
        return false;
    }
}

void ARoomSpawner::DeletePreviousRoom()
{
    if (oldRoom) {
        oldRoom->Destroy();
    }
    if (oldCorridor) {
        oldCorridor->Destroy();
    }
}


void ARoomSpawner::SpawnEnemies()
{
    TArray spawnPoints = currRoom->GetComponentsByTag(UChildActorComponent::StaticClass(), FName("SpawnPoint"));
    FActorSpawnParameters spawnParams;
    spawnParams.Owner = GetOwner();
    spawnParams.Instigator = nullptr;
    spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    for (int i = 0; i < currRoom->numOfEnemies; i++)
    {
        int randSpawnPoint = FMath::FRandRange(0, spawnPoints.Num());
        UChildActorComponent* MeshComp = Cast<UChildActorComponent>(spawnPoints[randSpawnPoint]);

        int randEnemySpawned = FMath::FRandRange(0, currRoom->enemiesToSpawn.Num());

        TSubclassOf<class AAICharacter> enemy = currRoom->enemiesToSpawn[randEnemySpawned];


        AAICharacter* enemySpawned = GetWorld()->SpawnActor<AAICharacter>(enemy, MeshComp->GetComponentLocation(), MeshComp->GetComponentRotation(), spawnParams);

    }
}

void ARoomSpawner::SpawnItem()
{
    TArray spawnPoints = currRoom->GetComponentsByTag(UChildActorComponent::StaticClass(), FName("SpawnPoint"));
    FActorSpawnParameters spawnParams;
    spawnParams.Owner = GetOwner();
    spawnParams.Instigator = nullptr;
    spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    int randSpawnPoint = FMath::FRandRange(0, spawnPoints.Num());
    UChildActorComponent* MeshComp = Cast<UChildActorComponent>(spawnPoints[randSpawnPoint]);

    int randItemSpawned = FMath::FRandRange(0, itemArray.Num());

    TSubclassOf<class AItem> item = itemArray[randItemSpawned];


    AItem* itemSpawned = GetWorld()->SpawnActor<AItem>(item, MeshComp->GetComponentLocation(), MeshComp->GetComponentRotation(), spawnParams);

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ITEM"));

}

void ARoomSpawner::SpawnShopItems(ARoom* shop)
{
    TArray spawnPoints = currRoom->GetComponentsByTag(UChildActorComponent::StaticClass(), FName("ItemSpawnPoint"));
    FActorSpawnParameters spawnParams;
    spawnParams.Owner = GetOwner();
    spawnParams.Instigator = nullptr;
    spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    int randSpawnPoint = FMath::FRandRange(0, spawnPoints.Num());
    UChildActorComponent* MeshComp = Cast<UChildActorComponent>(spawnPoints[randSpawnPoint]);

    int randItemSpawned = FMath::FRandRange(0, shopItemArray.Num());

    TSubclassOf<class AItem> item = shopItemArray[randItemSpawned];


    AItem* itemSpawned = GetWorld()->SpawnActor<AItem>(item, MeshComp->GetComponentLocation(), MeshComp->GetComponentRotation(), spawnParams);

    itemSpawned->hasSpawnedInShop = true;

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SHOP ITEM"));
}



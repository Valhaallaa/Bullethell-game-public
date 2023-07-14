// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.h"
#include "Item.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Math/UnrealMathUtility.h"
#include "AICharacter.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "RougeLike\RougeLikeGameMode.h"
#include "RoomSpawner.generated.h"

/*
USTRUCT()
struct FRoomData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Variables")
		float _PercentageChance;
	
	FRoomData() {

	}
};
*/
UCLASS()
class ROUGELIKE_API ARoomSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//FRoomData RoomData;


	void CalculateNewRoom();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Info")
		int normalRoomPercentage = 0;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Info")
		int shopRoomPercentage = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Info")
		int bossRoomPercentage = 0;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Info")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Info")
		TArray<TSubclassOf<class ARoom>> normalRooms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Info")
		TArray<TSubclassOf<class ARoom>> corridors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Info")
		TArray<TSubclassOf<class ARoom>> shopRooms;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Info")
		TArray<TSubclassOf<class ARoom>> bossRooms;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Info")
		TArray<TSubclassOf<class AItem>> itemArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Info")
		TArray<TSubclassOf<class AItem>> shopItemArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Info")
	ARoom* currRoom;

	ARoom* oldRoom;


	ARoom* newCorridor;

	ARoom* oldCorridor;

	ARoom* currCorridor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Info")
	int minRoomsBetweenShops;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Info")
	int minRoomsBetweenBoss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Info")
	int garuanteedShopNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Info")
		int garuanteedBossNum;

	int numSinceLastShop, numSinceLastBoss;

	int spawnPoint, roomNum;
	/*
	UENUM(BlueprintType, Category = "Room Info")
		enum class RoomID : uint8 {
		ID_NormalRoom      UMETA(DisplayName = "Normal Room"),
		ID_ShopRoom        UMETA(DisplayName = "Shop Room"),
		ID_BossRoom        UMETA(DisplayName = "Boss Room"),
	};
	*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//UFUNCTION(BlueprintCallable)
	//void SpawnRooms();
	UFUNCTION(BlueprintCallable)
		bool SpawnNewRoom(UActorComponent* exit, UStaticMeshComponent* door);
	UFUNCTION(BlueprintCallable)
		void DeletePreviousRoom();
	UFUNCTION(BlueprintCallable)
		void SpawnEnemies();
	UFUNCTION(BlueprintCallable)
		void SpawnItem();
	UFUNCTION(BlueprintCallable)
		void SpawnShopItems(ARoom* shop);
};



// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"


void AAICharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI OverlapEventbegin"));
		ABullet* Bull = Cast<ABullet>(OtherActor);
		if (Bull && Bull->GetShotByPlayer()) {
			AIHealth->TakeDamage(Bull->GetDamage());
		}
	}
}

void AAICharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Hitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hitbox"));
	Hitbox->InitCapsuleSize(44.f, 96.f);
	Hitbox->SetCollisionProfileName(TEXT("Trigger"));
	
	
	


	Hitbox->SetupAttachment(RootComponent);

	AIHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	AIHealth->SetMaxHealth(3.f);
	AIHealth->SetCurrentHealth(AIHealth->GetMaxHealth());
	//AIHealth->SetCurrencyValue(CurrencyValue);

	AIWeapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	AIWeapon->SetByPlayer(false);

	Hitbox->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnOverlapBegin);
	Hitbox->OnComponentEndOverlap.AddDynamic(this, &AAICharacter::OnOverlapEnd);

}




// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	ARougeLikeGameMode* gameMode = Cast<ARougeLikeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (gameMode)
	{
		gameMode->numOfEnemiesSpawned++;
	}
	
	
}
/*
void AAICharacter::Destroyed()
{
	ARougeLikeGameMode* gameMode = Cast<ARougeLikeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (gameMode)
	{
		gameMode->numOfEnemiesSpawned--;
		gameMode->AllEnemiesKilled();
	}
}
*/
// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AAICharacter::ShootAt(FVector TargetLocation)
{
	FVector direction = TargetLocation - GetActorLocation();
	direction.Normalize();

	FVector2D direction2D = FVector2D(direction.X, direction.Y);

	AIWeapon->AttemptShooting(direction2D);
	if (AIWeapon->GetAmmoCount() <= 0)
		AIWeapon->Reload();
}


// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "..\Public\WeaponComponent.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "RougeLike\RougeLikeCharacter.h"


// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	// ...

	maxAmmo = 6;
	currentAmmo = maxAmmo;
	bulletAmount = 1;
	speed = 5.f;
	bulletDamage = 1;
	sequentialShots = false;
	shootCooldown = 1.f;
	reloadTime = .5f;
	reloading = false;
	onCooldown = false;
	currentCooldown = 0;
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (reloading) {
		currentReloadTime += DeltaTime;
		if (currentReloadTime >= reloadTime) {
			reloading = false;
			currentAmmo = maxAmmo;
		}
	}
	if (onCooldown) {
		currentCooldown += DeltaTime;
		if (currentCooldown >= shootCooldown) {
			onCooldown = false;
			currentCooldown = 0;
		}
	}
}

void UWeaponComponent::AttemptShooting(FVector2D direction)
{
	if (currentAmmo == 0) {
		ARougeLikeCharacter* Char = Cast<ARougeLikeCharacter>(GetOwner());
		if (Char)
			Char->PlayerEmptyFeedback();
	}
	else if (!reloading && !onCooldown && !rapidFire) {
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = GetOwner();
		spawnParams.Instigator = nullptr;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		FVector shootingDirection = FVector(direction.X, direction.Y, 0);
		if (shootingDirection == FVector::ZeroVector) {
			shootingDirection = GetOwner()->GetActorForwardVector();
			

		}
	
		
		FVector spawnLocation = GetOwner()->GetActorLocation();
		FRotator spawnRotation = FRotator::ZeroRotator;
		currentAmmo--;
		onCooldown = true;
		bool Alternating = false;
		int offsetMulti=0;
		int offsetIncrease=0;
		

		
		switch (sequentialShots)
		{
		
		case true: // Pistol
			if (true) {
				ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), spawnLocation, spawnRotation, spawnParams); // add offset to rotation if done at once somehow
				Bullet->SetDamage(bulletDamage * 2);
				Bullet->SetDirection(shootingDirection);
				Bullet->SetSpeed(speed * 1.2f);
				Bullet->SetIfShotByPlayer(isPlayerWeapon);
				Bullet->SetupCollisions();
				ARougeLikeCharacter* Char = Cast<ARougeLikeCharacter>(GetOwner());
				if (Char)
					Char->PlayerPistolFeedback();
			}
			
			break;
		case false: // Shotgun
			if (true) {
				ARougeLikeCharacter* Char = Cast<ARougeLikeCharacter>(GetOwner());
				if (Char)
					Char->PlayerShotgunFeedback();
			}
			for (size_t i = 0; i < bulletAmount; i++)
			{
				ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), spawnLocation, spawnRotation, spawnParams); // add offset to rotation if done at once somehow
				Bullet->SetDamage(bulletDamage);
				FVector newDirection = shootingDirection;

				Bullet->SetSpeed(speed);
				Bullet->SetIfShotByPlayer(isPlayerWeapon);
				Bullet->SetupCollisions();
				//newDirection.GetSafeNormal();
				float checkNewDirectionX = newDirection.X;
				float checkNewDirectionY = newDirection.Y;
				if (newDirection.X < 0)
					checkNewDirectionX = -newDirection.X;
				if (newDirection.Y < 0)
					checkNewDirectionY = -newDirection.Y;

				if (!Alternating) {
					if (checkNewDirectionX > checkNewDirectionY)
						//newDirection.Y -= 0.25 * offsetMulti;
						newDirection = newDirection.RotateAngleAxis(10 * -offsetMulti,FVector(0,0,1));
					
					else
						//newDirection.X += 0.25 * offsetMulti;
						newDirection = newDirection.RotateAngleAxis(10 * offsetMulti, FVector(0, 0, 1));
					Alternating = true;
				}
				else {
					if (checkNewDirectionX > checkNewDirectionY)
						//newDirection.Y += 0.25 * offsetMulti;
						newDirection = newDirection.RotateAngleAxis(10 * offsetMulti, FVector(0, 0, 1));
					else {
						//newDirection.X -= 0.25 * offsetMulti;
						newDirection = newDirection.RotateAngleAxis(10 * -offsetMulti, FVector(0, 0, 1));
						
					}

					Alternating = false;
				}
					
				if (offsetIncrease == 1) {
					offsetIncrease = 0;
					offsetMulti++;
				}
				if (offsetMulti != 0)
					offsetIncrease++;
				else
					offsetMulti++;
					//newDirection.X = FMath::Clamp(newDirection.X, -1.f, 1.f);
					//newDirection.Y = FMath::Clamp(newDirection.Y, -1.f, 1.f);
			
				Bullet->SetDirection(newDirection);
			}
			break;
		}
		
		//ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), spawnLocation, spawnRotation, spawnParams);


	}
}

bool UWeaponComponent::GetShotgun()
{
	return sequentialShots;
}

int UWeaponComponent::GetAmmoCount()
{
	return currentAmmo;
}

void UWeaponComponent::SetByPlayer(bool Value)
{
	isPlayerWeapon = Value;
}

void UWeaponComponent::SetDamage(int Value)
{
	bulletDamage = Value;
}
int UWeaponComponent::GetDamage() {

	return bulletDamage;
}

int UWeaponComponent::GetMaxAmmo()
{
	return maxAmmo;
}

int UWeaponComponent::GetCurrentAmmo()
{
	return currentAmmo;
}

void UWeaponComponent::SetMaxAmmo(int Value)
{
	maxAmmo = Value;
}

void UWeaponComponent::SetShotgun(bool Value)
{
	sequentialShots = Value;
}

float UWeaponComponent::GetReloadTime()
{
	return reloadTime;
}

void UWeaponComponent::SetReloadTime(float Value)
{
	reloadTime = Value;
}

void UWeaponComponent::Reload()
{
	reloading = true;
	currentReloadTime = 0;
	ARougeLikeCharacter* Char = Cast<ARougeLikeCharacter>(GetOwner());
	if (Char)
		Char->PlayerReloadFeedback();
	

}

void UWeaponComponent::ClearCooldown() {
	onCooldown = false;
}

void UWeaponComponent::RapidFire(FVector Location, FRotator Rotation, FActorSpawnParameters SpawnParams, FVector Direction, int Remaining)
{
	ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), Location, Rotation, SpawnParams); // add offset to rotation if done at once somehow
	Bullet->SetDamage(bulletDamage);
	Bullet->SetDirection(Direction);
	Bullet->SetSpeed(speed);
	Bullet->SetIfShotByPlayer(isPlayerWeapon);
	Bullet->SetupCollisions();
	Remaining--;
}

void UWeaponComponent::EndRapid() {
	rapidFire = false;
}


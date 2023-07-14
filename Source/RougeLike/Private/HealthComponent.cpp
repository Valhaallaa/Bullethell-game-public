// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "AICharacter.h"
#include "RougeLike\RougeLikeCharacter.h"



// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	currentHealth = maxHealth;
	canTakeDamage = true;
	currentIFrameDuration = 0;
	iFrames = false;
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (iFrames) {
		currentIFrameDuration += DeltaTime;
		canTakeDamage = false;
		if (currentIFrameDuration >= iFrameDuration) {
			canTakeDamage = true;
			iFrames = false;
			currentIFrameDuration = 0;
		}
	}
	// ...
}

void UHealthComponent::SetMaxHealth(float Value)
{
	maxHealth = Value;
	
		currentHealth = maxHealth;
}

void UHealthComponent::SetCurrentHealth(float Value)
{
	currentHealth = Value;
	if (currentHealth > maxHealth)
		currentHealth = maxHealth;
}

void UHealthComponent::TakeDamage(float Value)
{
	int storedDamage =(int)Value;
	if (canTakeDamage && storedDamage > 0) {

		iFrames = true;
		if (currentArmour > 0) {
			if (currentArmour > Value) {
				currentArmour -= Value;
				storedDamage = 0;
			}
			else {
				storedDamage -= currentArmour;
				currentArmour = 0;
			}
		}
		if (currentArmour <= 0) {
			currentHealth -= storedDamage;
		}
		currentHealth = (int)(currentHealth + 0.5f);

		AAICharacter* AI = Cast<AAICharacter>(GetOwner());
		if (AI)
			AI->DamageFeedback();
		else {
			ARougeLikeCharacter* Char = Cast<ARougeLikeCharacter>(GetOwner());
			if (Char)
				Char->PlayerDamageFeedback();
		}
		if (currentHealth <= 0)
			Death();
	}
}

void UHealthComponent::IncreaseCurrentHealth(float Value)
{
	currentHealth += Value;
	if (currentHealth > maxHealth)
		currentHealth = maxHealth;
}

void UHealthComponent::IncreaseArmour(float Value)
{
	currentArmour += Value;
}

void UHealthComponent::AddDOTAffect()// the best MOBA
{

}

void UHealthComponent::Death()
{
	ARougeLikeGameMode* gameMode = Cast<ARougeLikeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (gameMode)
	{
		if (currencyValue > 0) {
			// Add currency addition here

			gameMode->numOfEnemiesSpawned--;
			gameMode->AllEnemiesKilled();
			gameMode->AddCoins(currencyValue);


			if (GetOwner()->ActorHasTag("Boss")) {
				gameMode->SaveInfo();

				UGameplayStatics::OpenLevel(this, "Forum");
			}

		}
		else {

			gameMode->SaveInfo();

			UGameplayStatics::OpenLevel(this, "Forum");

		}
	}
	GetOwner()->Destroy();
}
float UHealthComponent::GetCurrentHealth()
{
	return currentHealth;
}

float UHealthComponent::GetMaxHealth()
{
	return maxHealth;
}

float UHealthComponent::GetCurrentArmour()
{
	return currentArmour;
}

void UHealthComponent::SetTakeDamage(bool Value)
{
	canTakeDamage = Value;
}

void UHealthComponent::SetCurrencyValue(float Value)
{
	currencyValue = Value;
}



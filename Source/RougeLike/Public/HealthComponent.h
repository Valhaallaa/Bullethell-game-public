// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RougeLike\RougeLikeGameMode.h"


#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROUGELIKE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	float currentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	float maxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	float currentArmour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	float iFrameDuration;
	float currentIFrameDuration;
	bool iFrames;
	bool canTakeDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character,meta = (AllowPrivateAccess = "true"))
	bool isAlive;
	TArray<FVector2D> affectArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	float currencyValue;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable, Category = HealthComponent)
	void SetMaxHealth(float Value);
	UFUNCTION(BlueprintCallable, Category = HealthComponent)
	void SetCurrentHealth(float Value);
	UFUNCTION(BlueprintCallable, Category = HealthComponent)
	void TakeDamage(float Value);
	UFUNCTION(BlueprintCallable, Category = HealthComponent)
	void IncreaseCurrentHealth(float Value);
	UFUNCTION(BlueprintCallable, Category = HealthComponent)
	void IncreaseArmour(float Value);
	UFUNCTION(BlueprintCallable, Category = HealthComponent)
	void AddDOTAffect();
	UFUNCTION(BlueprintCallable, Category = HealthComponent)
	void Death();
	UFUNCTION(BlueprintCallable, Category = HealthComponent)
	float GetCurrentHealth();
	UFUNCTION(BlueprintCallable, Category = HealthComponent)
	float GetMaxHealth();
	UFUNCTION(BlueprintCallable, Category = HealthComponent)
	float GetCurrentArmour();
	UFUNCTION(BlueprintCallable, Category = HealthComponent)
	void SetTakeDamage(bool Value);
	UFUNCTION(BlueprintCallable, Category = HealthComponent)
	void SetCurrencyValue(float Value);

		
};

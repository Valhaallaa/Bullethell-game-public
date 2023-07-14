// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "WeaponComponent.h"
#include "Bullet.h"
#include "Components/CapsuleComponent.h"
#include "AICharacter.generated.h"

UCLASS()
class ROUGELIKE_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		class UHealthComponent* AIHealth;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
			class UCapsuleComponent* Hitbox;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
			class UWeaponComponent* AIWeapon;
		
//		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		//float CurrencyValue;

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//virtual void Destroyed();



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = AI)
	void ShootAt(FVector Target);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
		void DamageFeedback();

};

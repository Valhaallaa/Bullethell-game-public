// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "HealthComponent.h"
#include "RougeLike\RougeLikeGameMode.h"
#include "WeaponComponent.h"
#include "ShooterPawn.h"
#include "UpgradeComponent.h"
#include "GameFramework/Actor.h"
#include "RougeLikeCharacter.generated.h"


UCLASS(config=Game)
class ARougeLikeCharacter : public ACharacter
{
	GENERATED_BODY()

		bool isShooting;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		class UUpgradeComponent* UpgradeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* Hitbox;


	//class AShooterPawn* shootingDirectionPawn;
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	ARougeLikeCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		class UHealthComponent* PlayerHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		class UWeaponComponent* PlayerWeapon;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		FVector2D aimDirection;
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	void AimLeftRight(float Value);
	void AimUpDown(float Value);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	float dodgeCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	float currentDodgeCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	float dodgeForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool canDodge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	float dodgeDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	float currentDodgeDuration;


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	void Shoot();
	void StopShoot();
	void Reload();
	void Dodge();
	void Interact();
	void SwapWeapon();
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		AItem* itemHeld;

	UFUNCTION(BlueprintCallable)
	void SetItemHeld(AItem* newItem);


	AItem* GetItemHeld();
	*/
	void UseItem();

	UFUNCTION(BlueprintImplementableEvent)
		void DisplayUI();

	UFUNCTION(BlueprintImplementableEvent)
		void RemoveUI();

	AActor* actorOverlapping;
	bool isInInteractRange;


	UFUNCTION(BlueprintImplementableEvent)
		void PlayerDamageFeedback();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayerShotgunFeedback();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayerPistolFeedback();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayerEmptyFeedback();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayerReloadFeedback();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayerDodgeFeedback();
};


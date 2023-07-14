// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Bullet.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROUGELIKE_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Details", meta = (AllowPrivateAccess = "true"))
	int maxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Details", meta = (AllowPrivateAccess = "true"))
	int currentAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Details", meta = (AllowPrivateAccess = "true"))
	int bulletAmount;

	float currentCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Details", meta = (AllowPrivateAccess = "true"))
	float speed;
	bool onCooldown;
	bool isPlayerWeapon;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Details", meta = (AllowPrivateAccess = "true"))
	int bulletDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Details", meta = (AllowPrivateAccess = "true"))
	bool sequentialShots;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Details", meta = (AllowPrivateAccess = "true"))
	bool reloading;
	float currentReloadTime;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Details", meta = (AllowPrivateAccess = "true"))
	float reloadTime;
	bool rapidFire;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable, Category = WeaponComponent)
	void AttemptShooting(FVector2D direction);
	int GetAmmoCount();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Details", meta = (AllowPrivateAccess = "true"))
		float shootCooldown;
	void SetByPlayer(bool Value);
	UFUNCTION(BlueprintCallable, Category = WeaponComponent)
	void SetDamage(int Value);
	UFUNCTION(BlueprintCallable, Category = WeaponComponent)
	int GetDamage();
	UFUNCTION(BlueprintCallable, Category = WeaponComponent)
	int GetMaxAmmo();
	UFUNCTION(BlueprintCallable, Category = WeaponComponent)
	int GetCurrentAmmo();
	UFUNCTION(BlueprintCallable, Category = WeaponComponent)
	void SetMaxAmmo(int Value);
	UFUNCTION(BlueprintCallable, Category = WeaponComponent)
	void SetShotgun(bool Value);
	UFUNCTION(BlueprintCallable, Category = WeaponComponent)
	float GetReloadTime();
	UFUNCTION(BlueprintCallable, Category = WeaponComponent)
	void SetReloadTime(float Value);
	bool GetShotgun();
	UFUNCTION(BlueprintCallable, Category = WeaponComponent)
	void ClearCooldown();
	void RapidFire(FVector Location,FRotator Rotation, FActorSpawnParameters SpawnParams, FVector Direction, int Remaining);
	void EndRapid();
	void Reload();
	

		
};

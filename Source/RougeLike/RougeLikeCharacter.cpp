// Copyright Epic Games, Inc. All Rights Reserved.

#include "RougeLikeCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Bullet.h"
#include "Item.h"
#include "Shop.h"
#include "Room.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ARougeLikeCharacter

void ARougeLikeCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		ABullet* Bull = Cast<ABullet>(OtherActor);
		if (Bull) {
			if(Bull->GetShotByPlayer() == false)
			PlayerHealth->TakeDamage(Bull->GetDamage());
		}
	}
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->ActorHasTag("Interactable"))
		{
			AItem* item = Cast<AItem>(OtherActor);
			/*
			if (item->hasSpawnedInShop) {
			}
			*/
			actorOverlapping = OtherActor;
			isInInteractRange = true;
			DisplayUI();
		}
	}

}

void ARougeLikeCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->ActorHasTag("Interactable"))
		{
			actorOverlapping = nullptr;
			isInInteractRange = false;
			RemoveUI();
		}
	}
}

ARougeLikeCharacter::ARougeLikeCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;
	aimDirection = FVector2d(1, 0);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	PlayerHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	PlayerHealth->SetMaxHealth(3);
	PlayerHealth->SetCurrentHealth(PlayerHealth->GetMaxHealth());
	PlayerHealth->SetCurrencyValue(0.0f);

	PlayerWeapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	PlayerWeapon->SetByPlayer(true);

	//shootingDirectionPawn = CreateDefaultSubobject<AShooterPawn>(TEXT("ShootingDirection"));
	/*
	Hitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hitbox"));
	Hitbox->InitCapsuleSize(44.f, 96.f);
	Hitbox->SetCollisionProfileName(TEXT("Trigger"));
	Hitbox->SetupAttachment(RootComponent);
	*/
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARougeLikeCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ARougeLikeCharacter::OnOverlapEnd);


	dodgeCooldown = 3;
	currentDodgeCooldown = 0;
	canDodge = true;
	dodgeForce = 2000;
	currentDodgeDuration = 0;
	dodgeDuration = 0.5f;
	isShooting = false;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARougeLikeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ARougeLikeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ARougeLikeCharacter::MoveRight);


	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ARougeLikeCharacter::Shoot);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &ARougeLikeCharacter::StopShoot);
	PlayerInputComponent->BindAction("Item", IE_Released, this, &ARougeLikeCharacter::UseItem);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ARougeLikeCharacter::Reload);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ARougeLikeCharacter::Dodge);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ARougeLikeCharacter::Interact);
	PlayerInputComponent->BindAction("Swap", IE_Pressed, this, &ARougeLikeCharacter::SwapWeapon);


	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ARougeLikeCharacter::TurnAtRate);
	//PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ARougeLikeCharacter::LookUpAtRate);


	PlayerInputComponent->BindAxis("Aim Right / Left", this, &ARougeLikeCharacter::AimLeftRight);
	PlayerInputComponent->BindAxis("Aim Up / Down", this, &ARougeLikeCharacter::AimUpDown);
	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARougeLikeCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARougeLikeCharacter::TouchStopped);
}


void ARougeLikeCharacter::Shoot()
{
	isShooting = true;
	
	
}
void ARougeLikeCharacter::StopShoot()
{
	isShooting = false;
}

void ARougeLikeCharacter::Reload()
{
	PlayerWeapon->Reload();
}

void ARougeLikeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ...
	if(isShooting)
		PlayerWeapon->AttemptShooting(aimDirection);
	if (canDodge == false) {

		currentDodgeCooldown += DeltaTime;
		if (currentDodgeCooldown >= dodgeCooldown) {
			canDodge = true;
			currentDodgeCooldown = 0;
		}

	}
	if (currentDodgeDuration < dodgeDuration) {
		currentDodgeDuration += DeltaTime;
		if (currentDodgeDuration >= dodgeDuration)
			PlayerHealth->SetTakeDamage(true);
	}	
	
	ARougeLikeGameMode* gameMode = Cast<ARougeLikeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (gameMode)
	{
		int debugint = gameMode->numOfEnemiesSpawned;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(debugint.ToString()));
		UE_LOG(LogTemp, Warning, TEXT("The integer value is: %d"), debugint);


	}
}

void ARougeLikeCharacter::Dodge()
{
	if (canDodge) {
		canDodge = false;
		AddMovementInput(GetActorForwardVector(), dodgeForce);
		LaunchCharacter(GetActorForwardVector()*dodgeForce,true,true);
		currentDodgeDuration = 0;
		PlayerDodgeFeedback();
	}

}
/*
void ARougeLikeCharacter::SetItemHeld(AItem* newItem)
{
	
	ARougeLikeGameMode* gameMode = Cast<ARougeLikeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (gameMode)
	{
		gameMode->itemHeld = newItem;
	}
	
}

AItem* ARougeLikeCharacter::GetItemHeld()
{
	return itemHeld;
}
*/

void ARougeLikeCharacter::SwapWeapon()
{

	PlayerWeapon->SetShotgun(!PlayerWeapon->GetShotgun());
}


void ARougeLikeCharacter::UseItem()
{
	ARougeLikeGameMode* gameMode = Cast<ARougeLikeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (gameMode)
	{
		gameMode->UseHeldItem();
	}
}

void ARougeLikeCharacter::Interact()
{
	AShop* shop = Cast<AShop>(actorOverlapping);

	if (shop)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Shop"));
		shop->OpenShop();
	}

	AItem* item = Cast<AItem>(actorOverlapping);

	if (item) {
		RemoveUI();
		item->PickupItem();
		
	}

}

void ARougeLikeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ARougeLikeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ARougeLikeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ARougeLikeCharacter::AimLeftRight(float Value)
{
	
		aimDirection.X = Value;
	
}

void ARougeLikeCharacter::AimUpDown(float Value)
{
	
		aimDirection.Y = Value;
	

}

void ARougeLikeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ARougeLikeCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARougeLikeCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

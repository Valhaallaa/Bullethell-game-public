// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"






void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Bullet OverlapEventbegin"));
		
		ABullet* Bull = Cast<ABullet>(OtherActor);
		if (Bull != nullptr) {

		}
		else if(OtherActor->ActorHasTag("Player") && shotByPlayer){

		}
		else if (OtherActor->ActorHasTag("AI") && !shotByPlayer) {

		}
		else if (OtherActor->ActorHasTag("Trigger"))
		{
		}
		else
		{
			Destroy();
		}
	}
}

void ABullet::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	//Sphere->AttachToComponent(GetRootComponent());
	ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	Sphere->SetStaticMesh(SphereMeshAsset.Object);
	Sphere->SetWorldScale3D(FVector(0.3f));
	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	//UMaterialInterface bulletMaterial;
	
	static ConstructorHelpers::FObjectFinder<UMaterial> bulletMaterial(TEXT("Material'/Game/BulletMaterial.BulletMaterial'"));

	if (bulletMaterial.Succeeded())
		StoredMat = bulletMaterial.Object;
	DynamicMat = UMaterialInstanceDynamic::Create(StoredMat, Sphere);
	
	//UMaterialInterface* mat = bulletMaterial.Object;

	Sphere->SetMaterial(0, DynamicMat);
	RootComponent = Sphere;

	Hitbox = CreateDefaultSubobject<USphereComponent>(TEXT("Hitbox"));
	Hitbox->InitSphereRadius(5.f);
	Hitbox->SetCollisionProfileName(TEXT("OverlapAll"));
	Hitbox->SetupAttachment(RootComponent);

	

}


void ABullet::SetupCollisions() {
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ABullet::OnOverlapEnd);
}
// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector location = GetActorLocation();
	FVector NextLocation = location + (direction * speed);
	SetActorLocation(NextLocation);
	
}

void ABullet::SetDamage(int Value)
{
	bulletDamage = Value;

}

void ABullet::SetSpeed(float Value)
{
	speed = Value;
}

void ABullet::SetDirection(FVector Direction)
{
	direction = FVector(Direction.X,Direction.Y,0);
	//direction.Normalize();

}

void ABullet::SetIfShotByPlayer(bool Player)
{
	shotByPlayer = Player;
}

int ABullet::GetDamage()
{
	return bulletDamage;
}

bool ABullet::GetShotByPlayer()
{
	return shotByPlayer;
}

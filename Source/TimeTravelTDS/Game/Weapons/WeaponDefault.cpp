// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDefault.h"

// Sets default values
AWeaponDefault::AWeaponDefault()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initial setup of components
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;

	SkeletalMeshWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	SkeletalMeshWeapon->SetGenerateOverlapEvents(false);
	SkeletalMeshWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	SkeletalMeshWeapon->SetupAttachment(RootComponent);

	StaticMeshWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticMesh"));
	StaticMeshWeapon->SetGenerateOverlapEvents(false);
	StaticMeshWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMeshWeapon->SetupAttachment(RootComponent);

	ShootLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("ShootLocation"));
	ShootLocation->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeaponDefault::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponDefault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FireTick(DeltaTime);
}

// If weapon is about to fire and its NOT on shot cooldown then Fire()
void AWeaponDefault::FireTick(float DeltaTime)
{
	if(WeaponFiring)
	{
		if(FireTime < 0.f)
				Fire();
		else
			FireTime -= DeltaTime;
	}
}

// Weapon initialization, checking meshes
void AWeaponDefault::WeaponInit(FWeaponInfo WeaponInfo)
{
	if(SkeletalMeshWeapon && !SkeletalMeshWeapon->SkeletalMesh)
	{
		SkeletalMeshWeapon->DestroyComponent(true);
	}

	if(StaticMeshWeapon && !StaticMeshWeapon->GetStaticMesh())
	{
		StaticMeshWeapon->DestroyComponent();
	}

	WeaponData = WeaponInfo;
	RateOfFire = WeaponInfo.RateOfFire;
	
}

// Toggling the WeaponFiring boolean
void AWeaponDefault::SetWeaponStateFire(bool bIsFire)
{
	if(CheckWeaponCanFire())
		WeaponFiring = bIsFire;
	else
		WeaponFiring = false;
}

// Checking if weapon can fire
// TODO: Count the ammo and everything else ud need to check
bool AWeaponDefault::CheckWeaponCanFire()
{
	return true;
}

//TODO: Getter for projectile

void AWeaponDefault::Fire()
{
	FireTime = RateOfFire;
	if(ShootLocation)
	{
		// Setting the transform data to spawn Projectile
		FVector SpawnLocation = ShootLocation->GetComponentLocation();
		FRotator SpawnRotation = ShootLocation->GetComponentRotation();

		if(ProjectileActorClass)
		{
			//Spawning the projectile from ProjectileActorClass
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = GetInstigator();

			AProjectileDefault* myProjectile = Cast<AProjectileDefault>(GetWorld()->SpawnActor(ProjectileActorClass,&SpawnLocation,&SpawnRotation,SpawnParams));
			if(myProjectile)
			{
				// TODO: Include initial projectile settings it needs to keep with itself!
				myProjectile->InitialLifeSpan = 20.0f;
				myProjectile->ProjectileInit(WeaponData);
			}
		}
		else
		{
			
		}
	}
}

void AWeaponDefault::UpdateStateWeapon(EMovementState NewMovementState)
{
	//ToDo Dispersion
	ChangeDispersion();
}

void AWeaponDefault::ChangeDispersion()
{
}





// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDefault.h"


#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TimeTravelTDS/Character/TimeTravelTDSCharacter.h"

// TODO: Make global macro
#define GET_PLAYER_ZERO Cast<ATimeTravelTDSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))

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

	BulletCaseDropLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("BulletCaseDropLocation"));
	BulletCaseDropLocation->SetupAttachment(RootComponent);
	
	MagazineDropLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("MagazineDropLocation"));
	MagazineDropLocation->SetupAttachment(RootComponent);

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
	ReloadTick(DeltaTime);
	DispersionTick(DeltaTime);
}

// If weapon is about to fire and its NOT on shot cooldown then Fire()
void AWeaponDefault::FireTick(float DeltaTime)
{
	// Check if player has enough ammo
	if(FireTime <= 0.f)
	{
		if(WeaponFiring)
		{
			if(WeaponData.CurrentRound > 0 && Cast<ATimeTravelTDSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0))->DynamicSavedStats.Round > 0)
			{
				if(!WeaponReloading)
					Fire();
			}
			else
			{
				if(!WeaponReloading)
					InitReload();
			}
		}
	}	
	else
		FireTime -= DeltaTime;
}

void AWeaponDefault::ReloadTick(float DeltaTime)
{
	if(WeaponReloading)
	{
		if(ReloadTime < 0.0f)
		{
			FinishReload();
		}else
		{
			ReloadTime-=DeltaTime;
		}
	}
}

void AWeaponDefault::DispersionTick(float DeltaTime)
{
	if(DispersionTime <= 0)
	{
		if(!WeaponFiring || WeaponReloading)
		{
			if(ShouldReduceDispersion)
				CurrentDispersion = CurrentDispersion - CurrentDispersionReduction;
			else
				CurrentDispersion = CurrentDispersion + CurrentDispersionReduction;
		}

		if(CurrentDispersion < CurrentDispersionMin)
			CurrentDispersion = CurrentDispersionMin;
		else
		{
			if(CurrentDispersion > CurrentDispersionMax)
				CurrentDispersion = CurrentDispersionMax;
		}
	
		DispersionTime = DispersionReductionStepTime + CurrentDispersionTimeout;
		CurrentDispersionTimeout = 0;
	}
	else
		DispersionTime-=DeltaTime;
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
	ProjectileActorClass = WeaponInfo.ProjectileData.ProjectileClass;
	
	
}

// Toggling the WeaponFiring boolean
void AWeaponDefault::SetWeaponStateFire(bool bIsFire)
{
	if(CheckWeaponCanFire())
		WeaponFiring = bIsFire;
	else
		WeaponFiring = false;
}

float AWeaponDefault::GetCurrentDispersion()
{
	 return CurrentDispersion;
}

FVector AWeaponDefault::ApplyDispersionToShoot(const FVector DirectionShoot)
{
	return FMath::VRandCone(DirectionShoot, GetCurrentDispersion()*PI/180.f);
}

FVector AWeaponDefault::GetShootEndLocation()
{
	bool bShootDirection = false;
	FVector EndLocation = FVector(0.0f);
	FVector tmpV = ShootLocation->GetComponentLocation() - ShootEndLocation;

	if(tmpV.Size() > 100.0f)
	{
		EndLocation = ShootLocation->GetComponentLocation() + ApplyDispersionToShoot((ShootLocation->GetComponentLocation() - ShootEndLocation).GetSafeNormal()) * -20000.f;
	}
	else
	{
		EndLocation = ShootLocation->GetComponentLocation() + ApplyDispersionToShoot(ShootLocation->GetForwardVector()) * 20000.f; 
	}
	if(ShowDebug)
	{
		DrawDebugCone(GetWorld(), ShootLocation->GetComponentLocation(), -(ShootLocation->GetComponentLocation() - EndLocation),
			WeaponData.DistanceTrace,GetCurrentDispersion() * PI/100.f, GetCurrentDispersion()*PI/180.f,
            32, FColor::Emerald, false, .1f, static_cast<uint8>('\000'), 1.0f);
	}

	if(ShowDebug)
	{
		// Weapon Direction
		DrawDebugLine(GetWorld(), ShootLocation->GetComponentLocation(), ShootLocation->GetComponentLocation() + ShootLocation->GetForwardVector() * 500.0f, FColor::White,false, 5.f, (uint8)'\000', 0.5f);
		// Projectile Target Direction
		DrawDebugLine(GetWorld(), ShootLocation->GetComponentLocation(), ShootEndLocation, FColor::Blue, false, 5.f, (uint8)'\000', 0.5f);
		// Projectile Real Current Direction
		DrawDebugLine(GetWorld(), ShootLocation->GetComponentLocation(), EndLocation, FColor::Green,false, 5.f, (uint8)'\000', 0.5f);
		
	}

	return EndLocation;
}

// Checking if weapon can fire
// TODO: Count the ammo and everything else ud need to check
bool AWeaponDefault::CheckWeaponCanFire()
{
	return (Cast<ATimeTravelTDSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0))->DynamicSavedStats.Round > 0 && WeaponData.CurrentRound > 0 && !WeaponReloading);
}

//TODO: Getter for projectile
/**
 * @brief Function that handles firing projectiles from the gun, ammo management and other firing related activities
 *
 */
void AWeaponDefault::Fire()
{
	FireTime = RateOfFire;
	// Every shot is subtracted by one
	if(bIsControlledByPlayer)
		GET_PLAYER_ZERO->DynamicSavedStats.Round--;
	WeaponData.CurrentRound--;
	if (ShootLocation)
	{
		for(int8 i = 0; i < WeaponData.ProjectilesPerShot; i++)
		{
			// Setting the transform data to spawn Projectile
			FVector SpawnLocation = ShootLocation->GetComponentLocation();
			FRotator SpawnRotation = ShootLocation->GetComponentRotation();

			// Determine the projectile Direction, doesn't apply for Gamepad mode
			/**
			* TODO: Понять что с этим делать, т.к. подход не нравится.
			*  По сути надо постоянно брать точку куда указывает мышка, но у меня то идея брать не точку, а только направление,
			*  т.к. хочу сделать проект под геймпад. Надо подумать.
			*/
			if (!GET_PLAYER_ZERO->bIsGamepadUsed && bIsControlledByPlayer && WeaponData.bIsMouseFollowed)
			{
				FHitResult ResultHit;
				UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursorByChannel(
                    UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, ResultHit);
				FVector MouseCursorLocation = FVector(ResultHit.Location.X, ResultHit.Location.Y, GetActorLocation().Z);
				FVector Direction = MouseCursorLocation - SpawnLocation;
				Direction.Normalize();
				FMatrix myMatrix(Direction, FVector(0, 1, 0), FVector(0, 0, 1), FVector::ZeroVector);
				SpawnRotation = myMatrix.Rotator();
				if(CurrentDispersion != 0)
				{
					SpawnRotation.Yaw += FMath::RandRange(-CurrentDispersion,CurrentDispersion);
				}
			}


			if (ProjectileActorClass)
			{
				//Spawning the projectile from ProjectileActorClass
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = GetInstigator();

				if(CurrentDispersion != 0)
				{
					SpawnRotation.Yaw += FMath::RandRange(-CurrentDispersion,CurrentDispersion);
				}
				AProjectileDefault* myProjectile = Cast<AProjectileDefault>(
                    GetWorld()->SpawnActor(ProjectileActorClass, &SpawnLocation, &SpawnRotation, SpawnParams));
				if (myProjectile)
				{
					// TODO: Include initial projectile settings it needs to keep with itself!
					myProjectile->InitialLifeSpan = 20.0f;
					myProjectile->ProjectileInit(WeaponData);
				}
			
			}
			else
			{
				// If no projectile is setted, shoot a trace
				FHitResult HitResult;
				FVector StartLocation = ShootLocation->GetComponentLocation();
				FVector EndLocation = ShootLocation->GetForwardVector() * WeaponData.TraceRange + StartLocation;
				FCollisionQueryParams CollisionParams;
				CollisionParams.bTraceComplex = true;
				CollisionParams.bReturnPhysicalMaterial = true;

				//Create trace object in world
				bool TraceShot = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_PhysicsBody, CollisionParams);
				DrawDebugLine(
					GetWorld(),
					StartLocation,
					EndLocation,
					FColor(255,0,0),
					false,
					0.2,0,12.f);
				if(HitResult.GetActor())
				{
					if(HitResult.PhysMaterial.IsValid())
					{
						EPhysicalSurface HittedSurfaceType = UGameplayStatics::GetSurfaceType(HitResult);
						if(WeaponData.ProjectileData.HitDecalPerSurface.Contains(HittedSurfaceType))
						{
							UMaterialInterface* DecalMaterial = WeaponData.ProjectileData.HitDecalPerSurface[HittedSurfaceType];

							if(DecalMaterial && HitResult.GetComponent())
							{
								UE_LOG(LogTemp, Warning, TEXT("DECAL"));
								UGameplayStatics::SpawnDecalAttached(DecalMaterial,FVector(20.0f), HitResult.GetComponent(), NAME_None, HitResult.ImpactPoint, HitResult.Normal.Rotation(), EAttachLocation::KeepWorldPosition, 10.0f);
							}
						}
						if(WeaponData.ProjectileData.HitParticlePerSurface.Contains(HittedSurfaceType))
						{
							UParticleSystem* ParticleSystem = WeaponData.ProjectileData.HitParticlePerSurface[HittedSurfaceType];
							if(ParticleSystem)
							{
								UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, FTransform(HitResult.ImpactNormal.Rotation(), HitResult.ImpactPoint,FVector(1.0f)));
							}		
						}
						if(WeaponData.ProjectileData.HitSound)
						{
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponData.ProjectileData.HitSound, HitResult.ImpactPoint);
						}
					}
					
					UGameplayStatics::ApplyDamage(HitResult.GetActor(), WeaponDamage, GetInstigatorController(), this, NULL);
				}
				if(TraceShot)
				{
					UE_LOG(LogTemp, Warning, TEXT("AWeaponDefault::Fire() -- Shooted Actor: %s"), *FString(HitResult.GetActor()->GetName()));
					UE_LOG(LogTemp, Warning, TEXT("AWeaponDefault::Fire() -- Missing ProjectileActorClass reference, using trace"));
				}
				
				
			}
		}
	}
	/**
	 * Spawning the bullet case when shot is fired #animations
	 */
	if(WeaponData.SleeveBullets)
	{
		FActorSpawnParameters SpawnParams;
		FVector SpawnLocation = BulletCaseDropLocation->GetComponentLocation();
		UE_LOG(LogTemp, Warning, TEXT("Loc: %.2f"), SpawnLocation.X);
		FRotator SpawnRotation = FRotator(0.0f);
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetInstigator();
		AActor* BulletCaseDrop = Cast<AActor>(GetWorld()->SpawnActor(WeaponData.SleeveBullets, &SpawnLocation, &SpawnRotation, SpawnParams));
		if(BulletCaseDrop)
		{
			UStaticMeshComponent* BulletMeshComp = Cast<UStaticMeshComponent>(BulletCaseDrop->GetRootComponent());
			// Applying physics impulse on the case to the right from character
			FVector FwdVec = BulletCaseDrop->GetActorRightVector();
			FVector ImpulseVector = FVector(0.0f, 1.0f, 0.0f);
			float ImpulseStrength = 150.f;
			BulletMeshComp->AddImpulse(FwdVec * ImpulseStrength * BulletMeshComp->GetMass());
		}		
	}
	
	CurrentDispersion+=CurrentDispersionRecoil; // -- Add recoil to dispersion
	CurrentDispersionTimeout = WeaponData.DispersionWeaponData.DispersionTimeout;
	//TODO: Sounds and effects
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponData.SoundFireWeapon,GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData.EffectFireWeapon, ShootLocation->GetComponentTransform());
}

void AWeaponDefault::UpdateStateWeapon(EMovementState NewMovementState)
{
	if(CurrentMovementState != NewMovementState)
	{
		CurrentMovementState = NewMovementState;
		switch(NewMovementState)
		{
		case EMovementState::IDLE_STATE:
			ShouldReduceDispersion = true;
			CurrentDispersionMax = WeaponData.DispersionWeaponData.Idle_DispersionMax;
			CurrentDispersionMin = WeaponData.DispersionWeaponData.Idle_DispersionMin;
			CurrentDispersionRecoil = WeaponData.DispersionWeaponData.Idle_DispersionRecoil;
			CurrentDispersionReduction = WeaponData.DispersionWeaponData.Idle_DispersionReduction;
			break;
		case EMovementState::WALK_STATE:
			ShouldReduceDispersion = true;
			CurrentDispersionMax = WeaponData.DispersionWeaponData.Walk_DispersionMax;
			CurrentDispersionMin = WeaponData.DispersionWeaponData.Walk_DispersionMin;
			CurrentDispersionRecoil = WeaponData.DispersionWeaponData.Walk_DispersionRecoil;
			CurrentDispersionReduction = WeaponData.DispersionWeaponData.Walk_DispersionReduction;
			break;
		case EMovementState::AIM_STATE:
			ShouldReduceDispersion = true;
			CurrentDispersionMax = WeaponData.DispersionWeaponData.Aim_DispersionMax;
			CurrentDispersionMin = WeaponData.DispersionWeaponData.Aim_DispersionMin;
			CurrentDispersionRecoil = WeaponData.DispersionWeaponData.Aim_DispersionRecoil;
			CurrentDispersionReduction = WeaponData.DispersionWeaponData.Aim_DispersionReduction;
			break;
		case EMovementState::RUN_STATE:
			ShouldReduceDispersion = false;
			CurrentDispersionMax = WeaponData.DispersionWeaponData.Run_DispersionMax;
			CurrentDispersionMin = WeaponData.DispersionWeaponData.Run_DispersionMin;
			CurrentDispersionRecoil = WeaponData.DispersionWeaponData.Run_DispersionRecoil;
			CurrentDispersionReduction = WeaponData.DispersionWeaponData.Run_DispersionReduction;
			break;
		default:
			break;
		}
	}
}

void AWeaponDefault::ChangeDispersion()
{
}

void AWeaponDefault::InitReload()
{
	if(Cast<ATimeTravelTDSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0))->DynamicSavedStats.Round > 0)
	{
		WeaponReloading = true;
		ReloadTime = WeaponData.ReloadTime;
	}
	// TODO: Animations, sound, UI
	// Mag drop!
	if(WeaponData.MagazineDrop)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetInstigator();
		FVector SpawnLocation = MagazineDropLocation->GetComponentLocation();
		FRotator SpawnRotator = GetActorRotation();
		AActor* MagActor = Cast<AActor>(GetWorld()->SpawnActor(WeaponData.MagazineDrop, &SpawnLocation, &SpawnRotator, SpawnParams));
	}
	// Animation delegate call
	OnWeaponReloadStart.Broadcast(WeaponData.AnimCharReload);
}

void AWeaponDefault::FinishReload()
{
	WeaponReloading = false;
	int32 EquippedRounds = Cast<ATimeTravelTDSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0))->DynamicSavedStats.Round;
	// If we have less than max rounds in equipment then use all equipment ammo.
	WeaponData.CurrentRound = (EquippedRounds >= WeaponData.MaxRound) ? WeaponData.MaxRound : EquippedRounds;
	// TODO: Animations, sound, UI
	// Animation delegate call
	OnWeaponReloadEnd.Broadcast();
}

// Getters and setters
int32 AWeaponDefault::GetNumOfProjectilesPerShot()
{
	return WeaponData.ProjectilesPerShot;
}





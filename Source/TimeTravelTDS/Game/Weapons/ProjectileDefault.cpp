// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileDefault.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileDefault::AProjectileDefault()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initializing default components!
	BulletCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	BulletCollisionSphere->SetSphereRadius(16.0f);

	BulletCollisionSphere->bReturnMaterialOnMove = true; //<-- Hit even returns Phys Material!!!
	BulletCollisionSphere->SetCanEverAffectNavigation(false);//<-- Projectiles should not affect AI navigation

	//Setting The Root
	RootComponent = BulletCollisionSphere;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetCanEverAffectNavigation(false);

	BulletFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileParticle"));
	BulletFX->SetupAttachment(RootComponent);

	//TODO: Dont forget to create Projectile hit sounds and effects!

	BulletProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Data"));
	BulletProjectileMovement->UpdatedComponent = RootComponent;
	
	

}

// Called when the game starts or when spawned
void AProjectileDefault::BeginPlay()
{
	Super::BeginPlay();
	// Mapping functions for onHit, onOverlap and onEndOverlap
	BulletCollisionSphere->OnComponentHit.AddDynamic(this,&AProjectileDefault::BulletCollisionSphereHit);
	BulletCollisionSphere->OnComponentBeginOverlap.AddDynamic(this,&AProjectileDefault::BulletCollisionSphereBeginOverlap);
	BulletCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AProjectileDefault::BulletCollisionSphereEndOverlap);
	
}

// Called every frame
void AProjectileDefault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileDefault::ProjectileInit(FWeaponInfo WeaponInfo)
{
	BulletProjectileMovement->InitialSpeed = WeaponInfo.ProjectileInitialSpeed;
	BulletProjectileMovement->MaxSpeed = WeaponInfo.ProjectileMaxSpeed;
	BulletProjectileMovement->bRotationFollowsVelocity = true;
	BulletProjectileMovement->bShouldBounce = true;
	BulletProjectileMovement->ProjectileGravityScale = WeaponInfo.ProjectileGravityScale;

	ProjectileData = WeaponInfo.ProjectileData;
	
}


void AProjectileDefault::BulletCollisionSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor && Hit.PhysMaterial.IsValid())
	{
		EPhysicalSurface HittedSurfaceType = UGameplayStatics::GetSurfaceType(Hit);
		if(ProjectileData.HitDecalPerSurface.Contains(HittedSurfaceType))
		{
			UMaterialInterface* DecalMaterial = ProjectileData.HitDecalPerSurface[HittedSurfaceType];

			if(DecalMaterial && OtherComp)
			{
				UE_LOG(LogTemp, Warning, TEXT("DECAL"));
				UGameplayStatics::SpawnDecalAttached(DecalMaterial,FVector(20.0f), OtherComp, NAME_None, Hit.ImpactPoint, Hit.Normal.Rotation(), EAttachLocation::KeepWorldPosition, 10.0f);
			}
		}
		if(ProjectileData.HitParticlePerSurface.Contains(HittedSurfaceType))
		{
			UParticleSystem* ParticleSystem = ProjectileData.HitParticlePerSurface[HittedSurfaceType];
			if(ParticleSystem)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, FTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint,FVector(1.0f)));
			}		
		}
		if(ProjectileData.HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileData.HitSound, Hit.ImpactPoint);
		}

		UGameplayStatics::ApplyDamage(OtherActor, ProjectileData.ProjectileDamage, GetInstigatorController(), this, NULL);
	}
}

void AProjectileDefault::BulletCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AProjectileDefault::BulletCollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"

#include "Types.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	AIM_STATE UMETA(DisplayName = "AIM_STATE"),
	WALK_STATE UMETA(DisplayName = "WALK_STATE"),
	RUN_STATE UMETA(DisplayName = "RUN_STATE"),
	IDLE_STATE UMETA(DisplayName = "IDLE_STATE"),
	JUMP_STATE UMETA(DisplayName = "JUMP_STATE"),
	SLOWED_STATE UMETA(DisplayName = "SLOWED_STATE")
};

USTRUCT(BlueprintType)
struct FWeaponDispersion
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float Aim_DispersionMax = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float Aim_DispersionMin = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float Aim_DispersionRecoil = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float Aim_DispersionReduction = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float Walk_DispersionMax = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float Walk_DispersionMin = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float Walk_DispersionRecoil = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float Walk_DispersionReduction = 0.3f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float Run_DispersionMax = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float Run_DispersionMin = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float Run_DispersionRecoil = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float Run_DispersionReduction = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float Idle_DispersionMax = .5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float Idle_DispersionMin = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float Idle_DispersionRecoil = .4f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float Idle_DispersionReduction = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float DispersionTimeout = 2.f;
};

USTRUCT(BlueprintType)
struct FExplosionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Explosion Damage")
	float MaxDamage = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Explosion Damage")
	float MinDamage = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Explosion Radius")
	float InnerRadius = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Explosion Radius")
	float OuterRadius = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Explosion Damage")
	float DamageFalloff = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Explosion Physics")
	float ImpulseForce = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Explosion Physics")
	bool bIgnoreOwningActor = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Explosion Physics")
	bool ImpulseVelChange = false;
	
	
};

USTRUCT(BlueprintType)
struct FProjectileInfo
{
	GENERATED_BODY()

	// General settings that projectile needs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileData")
	TSubclassOf<class AProjectileDefault> ProjectileClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ProjectileData")
	UStaticMesh* ProjectileMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ProjectileData")
	UParticleSystem* TrailFX = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileData")
	float ProjectileDamage = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileData")
	float ProjectileLifeTime = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileData")
	float ProjectileInitSpeed = 2000.0f;

	
	// Material Decal on Hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileData")
	TMap<TEnumAsByte<EPhysicalSurface>, UMaterialInterface*> HitDecalPerSurface;
	
	// Sound on Hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileData")
	USoundBase* HitSound = nullptr;
	
	// Effect when Hit check by surface
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileData")
	TMap<TEnumAsByte<EPhysicalSurface>, UParticleSystem*> HitParticlePerSurface;

	// If Projectile is explosive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileData Explosion")
	bool isExplosive = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileData Explosion")
	FExplosionInfo ExplosionData;
};

USTRUCT(BlueprintType)
struct FWeaponInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Class")
		TSubclassOf<class AWeaponDefault> WeaponClass = nullptr;

	// Stats of a weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	int32 ProjectilesPerShot = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	float RateOfFire = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	float ReloadTime = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	int32 MaxRound = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	int32 CurrentRound = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
	FWeaponDispersion DispersionWeaponData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	FProjectileInfo ProjectileData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	float TraceRange;

	// Sounds of weapon. Sound of projectile hits will be in Projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
	USoundBase* SoundFireWeapon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
	USoundBase* SoundReloadWeapon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
	USoundBase* SoundNoAmmo = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
	UParticleSystem* EffectFireWeapon = nullptr;

	/**
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	FProjectileInfoi ProjectileSetting;
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage and Projectile")
	float WeaponDamage = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage and Projectile")
	float ProjectileInitialSpeed = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage and Projectile")
	float ProjectileMaxSpeed = 1200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage and Projectile")
	float ProjectileGravityScale = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage and Projectile")
	bool bIsMouseFollowed = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trace")
	float DistanceTrace = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HitEffect")
	UDecalComponent* DecalOnHit = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations")
	UAnimMontage* AnimCharFire = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations")
	UAnimMontage* AnimCharReload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visuals")
	TSubclassOf<AActor> MagazineDrop = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visuals")
	TSubclassOf<AActor> SleeveBullets = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visuals")
	UStaticMesh* SleeveBulletsMesh = nullptr;

};

//Dynamic Stats that will be saved and load dynamically throughout the game
USTRUCT(BlueprintType)
struct FSavedStats
{
	GENERATED_BODY()

	// Weapon related information
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	int32 Round = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<AWeaponDefault> WeaponToSaveLoad;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Stats")
	float Health = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Charactger Stats")
	int32 Money = 0; //BROKE SHIT U ARE
};

USTRUCT(BlueprintType)
struct FCharacterSpeed
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float AimSpeed = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float WalkSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float SlowedWalkSpeed = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float RunSpeed = 800.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float JumpVelocity = 420.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MidAirControl = 0.05f;
};

UCLASS()
class TIMETRAVELTDS_API UTypes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};

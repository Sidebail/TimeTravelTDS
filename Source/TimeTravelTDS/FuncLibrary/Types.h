// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
//#include "TimeTravelTDS/Game/Weapons/ProjectileDefault.h"

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
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float DispersionAimStart = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float DispersionAimMax = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float DispersionAimMin = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
		float DispersionAimShootCoef = 1.0f;
};

USTRUCT(BlueprintType)
struct FProjectileInfo
{
	GENERATED_BODY();

	// General settings that projectile needs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileData")
	TSubclassOf<class AProjectileDefault> ProjectileClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileData")
	float ProjectileDamage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileData")
	float ProjectileLifeTime = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileData")
	float ProjectileInitSpeed = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileVisuals")
	UParticleSystem* HitParticle = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileData")
	USoundBase* HitSound = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponInfo : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Class")
		TSubclassOf<class AWeaponDefault> WeaponClass = nullptr;

	// Stats of a weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	float RateOfFire = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	float ReloadTime = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	int32 MaxRound = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispersion")
	FWeaponDispersion DispersionWeaponData;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trace")
	float DistanceTrace = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HitEffect")
	UDecalComponent* DecalOnHit = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations")
	UAnimMontage* AnimCharFire = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations")
	UAnimMontage* AnimCharReload = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	UStaticMesh* MagazineDrop = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	UStaticMesh* SleeveBullets = nullptr;

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

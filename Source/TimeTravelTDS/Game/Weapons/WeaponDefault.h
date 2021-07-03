// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"

#include "ProjectileDefault.h"
//#include "TimeTravelTDS/FuncLibrary/Types.h"
#include "WeaponDefault.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponReloadStart, UAnimMontage*, Anim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponReloadEnd);

UCLASS()
class TIMETRAVELTDS_API AWeaponDefault : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponDefault();

	FOnWeaponReloadEnd OnWeaponReloadEnd;
	FOnWeaponReloadStart OnWeaponReloadStart;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class USceneComponent* SceneComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class USkeletalMeshComponent* SkeletalMeshWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class UStaticMeshComponent* StaticMeshWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class UArrowComponent* ShootLocation = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class UArrowComponent* BulletCaseDropLocation = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class UArrowComponent* MagazineDropLocation = nullptr;
	
	// Weapon Stats	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponStats")
		float WeaponDamage;
	UPROPERTY(BlueprintReadOnly, Category="WeaponStats")
		float RateOfFire;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponStats")
		FWeaponInfo WeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class TSubclassOf<class AProjectileDefault> ProjectileActorClass;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void FireTick(float DeltaTime);
	void ReloadTick(float DeltaTime);
	void DispersionTick(float DeltaTime);

	void WeaponInit(FWeaponInfo WeaponInfo);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireLogic")
	bool WeaponFiring = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireLogic")
	bool WeaponReloading = false;

	UFUNCTION(BlueprintCallable)
    void SetWeaponStateFire(bool bIsFire);

	// Utility for determining the end location of projectile
	FVector ShootEndLocation;
	float GetCurrentDispersion();
	FVector ApplyDispersionToShoot(const FVector DirectionShoot);
	
	UFUNCTION(BlueprintCallable)
		FVector GetShootEndLocation();
	
	// Variables for debug purposes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
		bool ShowDebug = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
		bool byBarrel = false; // If projectile is shooted by barrel...?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
		float SizeVectorToChangeShootDirectionLogic = 100.0f;

	// Dispersion control variables
	bool ShouldReduceDispersion = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="WeaponStats")
		float CurrentDispersion = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float CurrentDispersionMax = 1.0f;
	UPROPERTY(BlueprintReadOnly)
	float CurrentDispersionMin = 0.1f;
	UPROPERTY(BlueprintReadOnly)
	float CurrentDispersionRecoil = 0.1f;
	UPROPERTY(BlueprintReadOnly)
	float CurrentDispersionReduction = 0.1f;	

	bool CheckWeaponCanFire();

	void Fire();

	void UpdateStateWeapon(EMovementState NewMovementState);
	UPROPERTY(BlueprintReadOnly)
	EMovementState CurrentMovementState;
	void ChangeDispersion();

	//Timers'flags
	float FireTime = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category="Timed Actions")
	float ReloadTime = 0.0f;
	float DispersionTime = 0.3f;
	float DispersionReductionStepTime = 0.3f;
	float CurrentDispersionTimeout = 0;
	void InitReload();
	void FinishReload();

	// Getters and setters
	UFUNCTION(BlueprintCallable)
	int32 GetNumOfProjectilesPerShot();

	//This bool variable checks if weapon is used by player or not
	bool bIsControlledByPlayer = true;
	
};

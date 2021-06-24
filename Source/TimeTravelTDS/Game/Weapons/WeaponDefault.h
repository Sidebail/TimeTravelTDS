// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"

#include "ProjectileDefault.h"
//#include "TimeTravelTDS/FuncLibrary/Types.h"
#include "WeaponDefault.generated.h"

UCLASS()
class TIMETRAVELTDS_API AWeaponDefault : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponDefault();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class USceneComponent* SceneComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class USkeletalMeshComponent* SkeletalMeshWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class UStaticMeshComponent* StaticMeshWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class UArrowComponent* ShootLocation = nullptr;
	
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

	void WeaponInit(FWeaponInfo WeaponInfo);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireLogic")
	bool WeaponFiring = false;

	UFUNCTION(BlueprintCallable)
    void SetWeaponStateFire(bool bIsFire);

	bool CheckWeaponCanFire();

	/**
	FProjectileInfo GetProjectile();
	*/

	void Fire();

	void UpdateStateWeapon(EMovementState NewMovementState);
	void ChangeDispersion();

	//Timers'flags
	float FireTime = 0.0;
};

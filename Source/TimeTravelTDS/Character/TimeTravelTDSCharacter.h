// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimeTravelTDS/FuncLibrary/Types.h"
#include "TimeTravelTDSCharacter.generated.h"

UCLASS(Blueprintable)
class ATimeTravelTDSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATimeTravelTDSCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;


	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	


private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;
	


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		EMovementState MovementState = EMovementState::WALK_STATE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FCharacterSpeed MovementInfo;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
		FName FirstWeaponName;
	UPROPERTY(BlueprintReadOnly, Category="Equipment")
		AWeaponDefault* CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
	FSavedStats DynamicSavedStats;

	// Reload Animation handlers
	UFUNCTION()
		void WeaponReloadStart(UAnimMontage* Anim);
	UFUNCTION()
		void WeaponReloadEnd();
	UFUNCTION(BlueprintNativeEvent)
        void WeaponReloadStart_BP(UAnimMontage* Anim);
	UFUNCTION(BlueprintNativeEvent)
        void WeaponReloadEnd_BP();


	UFUNCTION()
		void InputAxisY(float Value);
	UFUNCTION()
		void InputAxisX(float Value);
	UFUNCTION()
		void InputLookAxisX(float Value);
	UFUNCTION()
		void InputLookAxisY(float Value);

	UFUNCTION(BlueprintCallable)
		void InitWeapon(FName WeaponDataRowName);
	
	/** Returns CursorToWorld subobject **/
	UFUNCTION(BlueprintCallable)
		FVector GetCursorToWorld();
	/** Does aiming logic */
	UFUNCTION()
		void TakeAim(bool bIsAiming);
	
	float AxisX = 0.0f;
	float AxisY = 0.0f;

	float LookAxisX = 0.0f;
	float LookAxisY = 0.0f;

	float bIsGamepadUsed = true;
	
	//Tick Func
	void MovementTick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		void CharacterUpdate();
	UFUNCTION(BlueprintCallable)
		void ChangeMovementState(EMovementState NewMovementState);

	// Getters and setters
	UFUNCTION(BlueprintCallable)
	FSavedStats GetDynamicStats();

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentAmmo();

	UFUNCTION(BlueprintCallable)
	void SetCurrentAmmo(int32 Ammo);

	// Input Related Functions
	UFUNCTION()
		void StartWalk();
	UFUNCTION()
		void StopWalk();
	UFUNCTION()
		void StartSprint();
	UFUNCTION()
		void StartShooting();
	UFUNCTION()
		void EndShooting();
	UFUNCTION(BlueprintCallable)
		void OnReloadInput();
	
	UFUNCTION()
		void SetMouseInput();
	UFUNCTION()
		void SetControllerInput();


	// DEBUG AND TRASH
	FVector LastClickLocation;
};


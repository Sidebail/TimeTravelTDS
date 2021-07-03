// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TimeTravelTDSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimeTravelTDS/Game/TDSGameInstance.h"
#include "TimeTravelTDS/Game/Weapons/WeaponDefault.h"

ATimeTravelTDSCharacter::ATimeTravelTDSCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	/**	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	*/

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	/* DEPRECATED

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Blueprints/PlayerCharacter/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	*/

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ATimeTravelTDSCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
	MovementTick(DeltaSeconds);
}

void ATimeTravelTDSCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitWeapon(FirstWeaponName);
}

void ATimeTravelTDSCharacter::SetupPlayerInputComponent(UInputComponent* NewInputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	NewInputComponent->BindAxis(TEXT("MoveForward"), this, &ATimeTravelTDSCharacter::InputAxisX);
	NewInputComponent->BindAxis(TEXT("MoveRight"), this, &ATimeTravelTDSCharacter::InputAxisY);
	NewInputComponent->BindAction(TEXT("WalkButton"), IE_Pressed, this, &ATimeTravelTDSCharacter::StartWalk);
	NewInputComponent->BindAction(TEXT("WalkButton"), IE_Released, this, &ATimeTravelTDSCharacter::StopWalk);
	NewInputComponent->BindAction(TEXT("AnyClick"), IE_Pressed, this, &ATimeTravelTDSCharacter::SetMouseInput);
	NewInputComponent->BindAction(TEXT("AnyGamepadButton"), IE_Pressed, this, &ATimeTravelTDSCharacter::SetControllerInput);
	NewInputComponent->BindAxis(TEXT("LookXAxis"), this, &ATimeTravelTDSCharacter::InputLookAxisX);
	NewInputComponent->BindAxis(TEXT("LookYAxis"), this, &ATimeTravelTDSCharacter::InputLookAxisY);

	NewInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATimeTravelTDSCharacter::StartShooting);
	NewInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ATimeTravelTDSCharacter::EndShooting);

	NewInputComponent->BindAction(TEXT("ReloadEvent"), EInputEvent::IE_Pressed, this, &ATimeTravelTDSCharacter::OnReloadInput);
	/*
	NewInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ATimeTravelTDSCharacter::StartSprint);
	NewInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ATimeTravelTDSCharacter::StopWalk);
	*/
}


void ATimeTravelTDSCharacter::InputAxisY(float Value)
{
	AxisY = Value;

}

void ATimeTravelTDSCharacter::InputAxisX(float Value)
{
	AxisX = Value;
}

void ATimeTravelTDSCharacter::InputLookAxisX(float Value)
{
	LookAxisX = Value;
}

void ATimeTravelTDSCharacter::InputLookAxisY(float Value)
{
	LookAxisY = Value;
}

void ATimeTravelTDSCharacter::InitWeapon(FName WeaponDataRowName)
{
	
	UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetGameInstance());
	FWeaponInfo newWeaponInfo;
	if(myGI)
	{
		if(myGI->GetWeaponInfoByName(WeaponDataRowName, newWeaponInfo))
		{
			if(newWeaponInfo.WeaponClass)
			{
				FVector SpawnLocation = FVector(0);
				FRotator SpawnRotation = FRotator(0);

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Instigator = GetInstigator();
				SpawnParams.Owner = GetOwner();

				AWeaponDefault* newWeapon = Cast<AWeaponDefault>(GetWorld()->SpawnActor(newWeaponInfo.WeaponClass, &SpawnLocation, &SpawnRotation, SpawnParams));
				if(newWeapon)
				{
					FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
					newWeapon->AttachToComponent(GetMesh(), Rule, FName("WeaponRightHand"));
					if(CurrentWeapon)
						CurrentWeapon->Destroy();
					CurrentWeapon = newWeapon;

					newWeapon->WeaponInit(newWeaponInfo);
					newWeapon->UpdateStateWeapon(MovementState);

					// Subscribe reloading delegates to animation handling
					newWeapon->OnWeaponReloadStart.AddDynamic(this, &ATimeTravelTDSCharacter::WeaponReloadStart);
					newWeapon->OnWeaponReloadEnd.AddDynamic(this, &ATimeTravelTDSCharacter::WeaponReloadEnd);
				}
			}
		}else
		{
			UE_LOG(LogTemp, Warning, TEXT("TimeTravelTDSCharacter::InitWeapon -- Weapon %s not found in weapon data table"));
		}
	}
}

void ATimeTravelTDSCharacter::WeaponReloadStart(UAnimMontage* Anim)
{
	WeaponReloadStart_BP(Anim);
}
void ATimeTravelTDSCharacter::WeaponReloadEnd()
{
	WeaponReloadEnd_BP();
}

void ATimeTravelTDSCharacter::WeaponReloadStart_BP_Implementation(UAnimMontage* Anim)
{
	// in BP
}
void ATimeTravelTDSCharacter::WeaponReloadEnd_BP_Implementation()
{
	// in BP
}


FVector ATimeTravelTDSCharacter::GetCursorToWorld()
{
	APlayerController* myController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (myController)
	{
		if(bIsGamepadUsed)
		{
			return FVector(0,0,0);
		}else
		{
			FHitResult ResultHit;
			myController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, ResultHit);
			return ResultHit.Location;
		}
	}else
	{
		return FVector(0,0,0);
	}
}

void ATimeTravelTDSCharacter::TakeAim(bool bIsAiming)
{
	if(bIsAiming)
	{
		ChangeMovementState(EMovementState::AIM_STATE);
		
	}
}

/**
 * @brief This function cares about the locomotion and rotation, regarding player input
 * @param DeltaTime 
 * 
 */
void ATimeTravelTDSCharacter::MovementTick(float DeltaTime)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisX);
	AddMovementInput(FVector(0.0f, 1.0f, 0.0f), AxisY);

	APlayerController* myController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (myController)
	{
		FHitResult ResultHit;
		if(bIsGamepadUsed)
		{
			// This IF works like a dead zone for a stick, so character wont be turned to the right when player is not touching the right stick
			if(UKismetMathLibrary::Abs(LookAxisX) > 1 || UKismetMathLibrary::Abs(LookAxisY) > 1)
			{
				FRotator newRotation = UKismetMathLibrary::MakeRotFromX(FVector(LookAxisX, LookAxisY, 0));
				//Adjusting the direction
				newRotation.Yaw+=90;
				FRotator lerpedRotation = UKismetMathLibrary::RInterpTo(GetActorRotation(),newRotation,DeltaTime,10);
				SetActorRotation(lerpedRotation);
			}
		}else
		{
			// If controller is not used - rotate character according to mouse 
			myController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, ResultHit);
			SetActorRotation(FQuat(FRotator(0.0f, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw ,0.0f)));
		}

		// Update weapon's logic according to player's movement state
		if(CurrentWeapon)
			CurrentWeapon->UpdateStateWeapon(MovementState);

		// After rotation let's fix the weapon firing direction
		// Честно не вижу зачем это. Привязка к сокету уже дает необходимый подъём оружия...
		// Мы же не будем создавать настолько мелкие цели в ТопДаунШутере, в которых пара пикселей по Z решит исход попадания...
		// TODO: Figure this out or clean
		/**
		if(CurrentWeapon)
		{
			FVector Displacement = FVector(0);
			switch (MovementState)
			{
				case EMovementState::AIM_STATE:
					Displacement = FVector(0.0f, 0.0f, 160.0f);
					break;
				case EMovementState::WALK_STATE:
					Displacement = FVector(0.0f, 0.0f, 120.0f);
					break;
				case EMovementState::RUN_STATE:
					Displacement = FVector(0.0f, 0.0f, 120.0f);
					break;
				default:
					break;
			}
			CurrentWeapon->loca = ResultHit.Location + Displacement;
		}
		*/
		
		
	}
}

void ATimeTravelTDSCharacter::CharacterUpdate()
{
	float ResultSpeed = 600.0f;
	switch (MovementState)
	{
	case EMovementState::AIM_STATE: 
		ResultSpeed = MovementInfo.AimSpeed;
		break;
	case EMovementState::WALK_STATE:
		ResultSpeed = MovementInfo.WalkSpeed;
		break;
	case EMovementState::RUN_STATE:
		ResultSpeed = MovementInfo.RunSpeed;
		break;
	case EMovementState::SLOWED_STATE:
		ResultSpeed = MovementInfo.SlowedWalkSpeed;
		break;
	default:
		break;
	}

	GetCharacterMovement()->MaxWalkSpeed = ResultSpeed;
}

void ATimeTravelTDSCharacter::ChangeMovementState(EMovementState NewMovementState)
{
	MovementState = NewMovementState;
	CharacterUpdate();
}

FSavedStats ATimeTravelTDSCharacter::GetDynamicStats()
{
	return DynamicSavedStats;
}

int32 ATimeTravelTDSCharacter::GetCurrentAmmo()
{
	return DynamicSavedStats.Round;
}

void ATimeTravelTDSCharacter::SetCurrentAmmo(int32 Ammo)
{
	DynamicSavedStats.Round = Ammo;
}

void ATimeTravelTDSCharacter::StartWalk()
{
	ChangeMovementState(EMovementState::SLOWED_STATE);
}

void ATimeTravelTDSCharacter::StopWalk()
{
	ChangeMovementState(EMovementState::WALK_STATE);
}

void ATimeTravelTDSCharacter::StartSprint()
{
	ChangeMovementState(EMovementState::RUN_STATE);
}

void ATimeTravelTDSCharacter::StartShooting()
{
	CurrentWeapon->SetWeaponStateFire(true);
	APlayerController* myController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Setting the last click location for weapon logic and further debug and logging.
	if(bIsGamepadUsed)
	{
		FHitResult ResultHit;
		myController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, ResultHit);
		LastClickLocation = ResultHit.Location;
	}
	
			
}

void ATimeTravelTDSCharacter::EndShooting()
{
	CurrentWeapon->SetWeaponStateFire(false);
}

void ATimeTravelTDSCharacter::OnReloadInput()
{
	if(CurrentWeapon)
		CurrentWeapon->InitReload();
}

void ATimeTravelTDSCharacter::SetMouseInput()
{
	bIsGamepadUsed = false;
}

void ATimeTravelTDSCharacter::SetControllerInput()
{
	bIsGamepadUsed = true;
}



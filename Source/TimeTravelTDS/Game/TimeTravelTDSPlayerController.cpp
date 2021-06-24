// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TimeTravelTDSPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "TimeTravelTDS/Character/TimeTravelTDSCharacter.h"
#include "Engine/World.h"

ATimeTravelTDSPlayerController::ATimeTravelTDSPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ATimeTravelTDSPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

}

void ATimeTravelTDSPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	//InputComponent->BindAction("ResetVR", IE_Pressed, this, &ATimeTravelTDSPlayerController::OnResetVR);
}
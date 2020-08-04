// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TimeTravelTDSGameMode.h"
#include "TimeTravelTDSPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "TimeTravelTDS/Character/TimeTravelTDSCharacter.h"

ATimeTravelTDSGameMode::ATimeTravelTDSGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATimeTravelTDSPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/PlayerCharacter/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
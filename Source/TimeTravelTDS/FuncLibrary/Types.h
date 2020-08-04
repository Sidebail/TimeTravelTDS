// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
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

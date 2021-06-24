// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "../FuncLibrary/Types.h"

#include "TDSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TIMETRAVELTDS_API UTDSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponSetting")
    UDataTable* WeaponInfoTable = nullptr;
    
    UFUNCTION(BlueprintCallable)
    bool GetWeaponInfoByName(FName NameOfWeapon, FWeaponInfo& OutInfo);
};

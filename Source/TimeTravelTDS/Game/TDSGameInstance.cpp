// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSGameInstance.h"

bool UTDSGameInstance::GetWeaponInfoByName(FName NameOfWeapon, FWeaponInfo& OutInfo)
{
    bool bIsFound = false;
    FWeaponInfo* WeaponInfoRow;
    if(WeaponInfoTable)
    {
        WeaponInfoRow = WeaponInfoTable->FindRow<FWeaponInfo>(NameOfWeapon, "", false);

        if(WeaponInfoRow)
        {
            bIsFound = true;
            OutInfo = *WeaponInfoRow;
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UTDSGameInstance::GetWeaponInfoByName -- Missing WeaponTable pointer! NullException!"));
    }
   
    return bIsFound; 
}

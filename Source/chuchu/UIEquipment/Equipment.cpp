// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment.h"
#include "../UIItem/Inventory.h"
#include "Components/CanvasPanelSlot.h"
#include "../Player/MainPlayerController.h"
#include "../chuchuGameModeBase.h"

void UEquipment::NativeConstruct()
{
	Super::NativeConstruct();

	UEquipTile* EqTile;
	FString firText = "UI_EquipTile"; 
	for (int i = 1; i < 7; i++)
	{
		FString secText = firText + FString::FromInt(i);
		EqTile = Cast<UEquipTile>(GetWidgetFromName(*secText));
		EqTile->SetTileName(GetEnumToString((EEquipType)(i-1)));
		EqTile->SetInventoryclass(m_Inventoryclass);
		m_EquipTileArry.Add(EqTile);
	}

}

void UEquipment::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}


void UEquipment::InitEquipment()
{
	int32 Arraysize = m_EquipTileArry.Num();
	for (int i = 0; i < Arraysize; i++)
	{
		m_EquipTileArry[i]->SetInventoryclass(m_Inventoryclass);
	}
}


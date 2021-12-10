// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment.h"
#include "../UIItem/Inventory.h"
#include "Components/CanvasPanelSlot.h"
#include "../Player/MainPlayerController.h"

void UEquipment::NativeConstruct()
{
	Super::NativeConstruct();

	//m_EquipmentTile = Cast<UEquipTile>(GetWidgetFromName(TEXT("UI_EquipTile1")));
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

	m_CloseButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_55")));
	m_CloseButton->OnClicked.AddDynamic(this, &UEquipment::CloseButtonClick);
}

void UEquipment::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

// 마우스가 온 상태일때만
//FReply UEquipment::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	Super::NativeOnMouseMove(InGeometry, InMouseEvent);
//
//	return FReply::Handled();
//}

void UEquipment::CloseButtonClick()
{
	if (GetVisibility() == ESlateVisibility::Collapsed) //인벤토리가 가려져있을때 호출되면
		SetVisibility(ESlateVisibility::SelfHitTestInvisible); //인벤토리가 보이도록

	else
	{
		SetVisibility(ESlateVisibility::Collapsed);

		m_Inventoryclass->CloseInvenUI();
		APlayerController* PController = GetWorld()->GetFirstPlayerController();
		PController->SetInputMode(FInputModeGameOnly()); // 커서 없어지고 마우스 방향으로 카메라 회전
		PController->bShowMouseCursor = false;
	}
}

void UEquipment::CloseEquipUI()
{
	if (GetVisibility() == ESlateVisibility::Collapsed)
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UEquipment::InitEquipment()
{
	int32 Arraysize = m_EquipTileArry.Num();
	for (int i = 0; i < Arraysize; i++)
	{
		m_EquipTileArry[i]->SetInventoryclass(m_Inventoryclass);
	}
}
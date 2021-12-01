// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory.h"
#include "Components/CanvasPanelSlot.h"
#include "../chuchuGameInstance.h"
#include "../chuchuGameModeBase.h"
#include "../UIEquipment/Equipment.h"


void UInventory::NativeConstruct()
{
	Super::NativeConstruct();
	
	Capacity = 20;

	m_InventoryTile = Cast<UTileView>(GetWidgetFromName(TEXT("InventoryTile")));
	m_InventorySlot= Cast<UTileView>(GetWidgetFromName(TEXT("InventorySlot")));
	m_CloseButton = Cast<UButton>(GetWidgetFromName(TEXT("CloseButton")));
	m_ItemInfoWidget = Cast<UItemInfo>(GetWidgetFromName(TEXT("UI_ItemInfo")));

	m_InventoryTile->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	m_InventorySlot->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	UchuchuGameInstance* gameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());
	if (gameInst)
	{
		const FUIItemDataInfo* ItemInfo = gameInst->FindUIItemInfo("Sword");
		if (ItemInfo)
		{
			UItemData* ItemData = NewObject<UItemData>(this, UItemData::StaticClass());
			ItemData->SetNameText(ItemInfo->m_ItemName);
			ItemData->SetIconTex(ItemInfo->m_Thumbnail);
			ItemData->SetMeshPath(ItemInfo->m_MeshPath);
			ItemData->SetItemType(EItemType::Equip, EEquipType::WEAPON);
			ItemData->SetItemDesc(ItemInfo->m_ItemDesc);
			ItemData->SetPickMesh(ItemInfo->m_PickMesh);
			m_InventoryTile->AddItem(ItemData);
		}

		const FUIItemDataInfo* ItemInfo2 = gameInst->FindUIItemInfo("TopBody");
		if (ItemInfo2)
		{
			UItemData* ItemData1 = NewObject<UItemData>(this, UItemData::StaticClass());
			ItemData1->SetNameText(ItemInfo2->m_ItemName);
			ItemData1->SetIconTex(ItemInfo2->m_Thumbnail);
			ItemData1->SetMeshPath(ItemInfo2->m_MeshPath);
			ItemData1->SetItemType(EItemType::Equip, EEquipType::TOPBODY);
			ItemData1->SetItemDesc(ItemInfo->m_ItemDesc);
			ItemData1->SetPickMesh(ItemInfo2->m_PickMesh);
			m_InventoryTile->AddItem(ItemData1);
		}
	}

	m_MouseHovered = false;

	// dynamic
	m_InventoryTile->OnItemClicked().AddUObject(this, &UInventory::ItemClick);
	m_InventoryTile->OnItemIsHoveredChanged().AddUObject(this, &UInventory::ItemHOvered);
	m_CloseButton->OnClicked.AddDynamic(this, &UInventory::CloseButtonClick);
}

void UInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

// 마우스가 온 상태일때만
FReply UInventory::NativeOnMouseMove(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	/*
	* GetWorld()->GetFirstPlayerController()->GetMousePosition(); //마우스 위치값을 얻어올 숭 ㅣㅆ따.
	*/

	if (m_MouseHovered)
	{
		//Vector2D
		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(m_ItemInfoWidget->Slot); //슬롯을 패널로 변환

		if (PanelSlot)
		{
			FVector2D	WidgetPos = InMouseEvent.GetScreenSpacePosition();

			//USlateBlueprintLibrary::ScreenToWidgetLocal(this, InGeometry,
			//	InMouseEvent.GetScreenSpacePosition(), WidgetPos);

			// 마우스의 Screen좌표를 위젯의 Local 좌표로 변경한다.
			WidgetPos = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry,
				WidgetPos);

			WidgetPos.X += 1.f;
			WidgetPos.Y += 1.f;

			PanelSlot->SetPosition(WidgetPos); //마우스위치를 반대로세팅

			//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("X : %.5f Y : %.5f"), WidgetPos.X, WidgetPos.Y));
		}
	}

	return FReply::Handled();
}

void UInventory::ItemClick(UObject* Data)
{
	if (Data)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		UItemData* itemData = Cast<UItemData>(Data);

		//장착하기 전에 이전 데이터 먼저 받아옴(이전에 장비가 있던 상태인지 아닌지의 데이터)
		UEquipTile* oldItem = m_Equipclass->GetEquipTileArray()[(int)itemData->GetEquipType()];
		bool IsEquiped = oldItem->GetIsEquip();

		//장착 후 IsEquip이 true로 데이터가 바뀜
		itemData->SetInventory(this);
		itemData->Use(Player);

		// 장착한 아이템이 존재했을 때 다시 인벤토리로 반환
		if (IsEquiped)
		{
			const FString ItemOldName = oldItem->GetItemOldName();
			UchuchuGameInstance* gameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());
			if (gameInst)
			{
				const FUIItemDataInfo* ItemInfo = gameInst->FindUIItemInfo(ItemOldName);
				if (ItemInfo)
				{
					UItemData* ItemData = NewObject<UItemData>(this, UItemData::StaticClass());
					ItemData->SetNameText(ItemInfo->m_ItemName);
					ItemData->SetIconTex(ItemInfo->m_Thumbnail);
					ItemData->SetMeshPath(ItemInfo->m_MeshPath);
					ItemData->SetItemDesc(ItemInfo->m_ItemDesc);
					ItemData->SetItemType(EItemType::Equip, ItemInfo->m_EquipType);
					ItemData->SetPickMesh(ItemInfo->m_PickMesh);
					AddItem(ItemData);
				}
			}
		}

		RemoveItem(Data);
	}
}

void UInventory::ItemScroll(UObject* Data, UUserWidget* widget)
{
}

void UInventory::ItemHOvered(UObject* Data, bool Hovered)
{
	m_MouseHovered = Hovered;

	if (Hovered)
	{
		m_ItemInfoWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible); // 패널 앞으로
		UItemData* Item = Cast<UItemData>(Data);

		if (Item)
		{
			m_ItemInfoWidget->SetItemName(Item->GetNameText());
			m_ItemInfoWidget->SetItemTex(Item->GetIconTex());
			m_ItemInfoWidget->SetItemInfo(Item->GetItemDesc());
			//PrintViewport(1.f, FColor::Red, Item->GetNameText());
		}
	}

	else
		m_ItemInfoWidget->SetVisibility(ESlateVisibility::Collapsed);

}

void UInventory::CloseButtonClick()
{
	if (GetVisibility() == ESlateVisibility::Collapsed) //인벤토리가 가려져있을때 호출되면
		SetVisibility(ESlateVisibility::SelfHitTestInvisible); //인벤토리가 보이도록

	else
		SetVisibility(ESlateVisibility::Collapsed);
}

bool UInventory::AddItem(UObject* Item)
{
	if (Items.Num() >= Capacity || !Item)
		return false;

	class UItemData* Itemdata = Cast<UItemData>(Item);

	Itemdata->OwningInventory = this;
	Itemdata->m_World = GetWorld();
	
	Items.Add(Itemdata);
	m_InventoryTile->AddItem(Item);

	return true;
}
bool UInventory::RemoveItem(UObject* Item)
{
	if (Item)
	{
		class UItemData* Itemdata = Cast<UItemData>(Item);
		Itemdata->OwningInventory = nullptr; // 인벤토리를 널
		Itemdata->m_World = nullptr;

		m_InventoryTile->RemoveItem(Item);
		Items.RemoveSingle(Itemdata);

		return true;
	}
	return false;
}
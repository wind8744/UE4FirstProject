// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory.h"
#include "Components/CanvasPanelSlot.h"
#include "../chuchuGameInstance.h"
#include "../chuchuGameModeBase.h"
#include "../UIEquipment/Equipment.h"
#include "../Player/MainPlayerController.h"
#include "../Player/PlayerCharacter.h"

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();
	
	Capacity = 20;

	m_InventoryTile = Cast<UTileView>(GetWidgetFromName(TEXT("InventoryTile")));
	m_InventorySlot= Cast<UTileView>(GetWidgetFromName(TEXT("InventorySlot")));
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

	m_ManaItem = false;
	m_HealthItem = false;

	// dynamic
	m_InventoryTile->OnItemClicked().AddUObject(this, &UInventory::ItemClick);
	m_InventoryTile->OnItemIsHoveredChanged().AddUObject(this, &UInventory::ItemHOvered);
}

void UInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

// ���콺�� �� �����϶���
FReply UInventory::NativeOnMouseMove(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	/*
	* GetWorld()->GetFirstPlayerController()->GetMousePosition(); //���콺 ��ġ���� ���� �� �Ӥ���.
	*/

	if (m_MouseHovered)
	{
		//Vector2D
		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(m_ItemInfoWidget->Slot); //������ �гη� ��ȯ

		if (PanelSlot)
		{
			FVector2D	WidgetPos = InMouseEvent.GetScreenSpacePosition();

			//USlateBlueprintLibrary::ScreenToWidgetLocal(this, InGeometry,
			//	InMouseEvent.GetScreenSpacePosition(), WidgetPos);

			// ���콺�� Screen��ǥ�� ������ Local ��ǥ�� �����Ѵ�.
			WidgetPos = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry,
				WidgetPos);

			WidgetPos.X += 1.f;
			WidgetPos.Y += 1.f;

			PanelSlot->SetPosition(WidgetPos); //���콺��ġ�� �ݴ�μ���

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

		if (itemData->GetItemType() == EItemType::Food)
		{
			itemData->Use(Player);
		}
		else
		{
			//�����ϱ� ���� ���� ������ ���� �޾ƿ�(������ ��� �ִ� �������� �ƴ����� ������)
			UEquipTile* oldItem = m_Equipclass->GetEquipTileArray()[(int)itemData->GetEquipType()];
			bool IsEquiped = oldItem->GetIsEquip();

			//���� �� IsEquip�� true�� �����Ͱ� �ٲ�
			itemData->SetInventory(this);
			itemData->Use(Player);

			// ������ �������� �������� �� �ٽ� �κ��丮�� ��ȯ
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
		m_ItemInfoWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible); // �г� ������
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

bool UInventory::AddItem(UObject* Item)
{
	if (Items.Num() >= Capacity || !Item)
		return false;

	class UItemData* Itemdata = Cast<UItemData>(Item);
	
	if (Itemdata->GetItemType() == EItemType::Food)
	{
		for (int a = 0; a < Items.Num(); ++a)
		{
			if (Itemdata->GetNameText().Equals(Items[a]->GetNameText(), ESearchCase::CaseSensitive))
			{
				FString test1 = Itemdata->GetNameText();
				PrintViewport(1.f, FColor::Red, test1);
				Items[a]->CountUp();
				return true;
			}
		}
	}

	Itemdata->OwningInventory = this;
	Itemdata->m_World = GetWorld();
	
	m_InventoryTile->AddItem(Item);
	Items.Add(Itemdata);
	return true;
}
bool UInventory::RemoveItem(UObject* Item)
{
	if (Item)
	{
		class UItemData* Itemdata = Cast<UItemData>(Item);

		if (Itemdata->GetItemType() == EItemType::Food)
		{
			for (int a = 0; a < Items.Num(); ++a)
			{
				if (Itemdata->GetNameText().Equals(Items[a]->GetNameText(), ESearchCase::CaseSensitive))
				{
					FString test1 = Itemdata->GetNameText();
					PrintViewport(1.f, FColor::Red, test1);
					int32 ret = Itemdata->CountDown();
					if (ret>=0) //0���� �ƴϸ� ���ڸ� �ٿ�
					{
						return true;
					}
				}
			}	
		}

		Itemdata->OwningInventory = nullptr; // �κ��丮�� ��
		Itemdata->m_World = nullptr;

		m_InventoryTile->RemoveItem(Item);
		Items.RemoveSingle(Itemdata);

		return true;
	}
	return false;
}

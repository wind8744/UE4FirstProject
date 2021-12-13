// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/TileView.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "ItemTile.h"
#include "ItemData.h"
#include "ItemInfo.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"
/**
 * 1. �κ��丮 ui Ŭ����
 * 2. �κ��丮 ui���� ǥ���� ������ui Ŭ����
 * 3. �κ��丮 ������ ui������ ��¥ item data
 */
UCLASS()
class CHUCHU_API UInventory : public UUserWidget // 1. �κ��丮 ui 
{
	GENERATED_BODY()

protected:
	// �κ��丮 ui ������ Ÿ�Ϻ� ���Ѱ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTileView* m_InventoryTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTileView* m_InventorySlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UItemInfo* m_ItemInfoWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_CloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 Capacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UItemData*> Items;

	TArray<UObject*>	m_SlotArray;

	int32	m_ItemCount;
	bool	m_MouseHovered;

	class UEquipment* m_Equipclass; //���â

public:
	void SetEquipClass(UEquipment* _class) { m_Equipclass = _class; }
	UEquipment* GetEquipClass() { return m_Equipclass; }

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	//widget Drag&drop
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation); //�巡�� ����

public:
	bool AddItem(UObject* Item);
	bool RemoveItem(UObject* Item);

public:
	//dynamic 
	UFUNCTION()
		void ItemClick(UObject* Data);

	UFUNCTION()
		void ItemScroll(UObject* Data, UUserWidget* widget);

	UFUNCTION()
		void ItemHOvered(UObject* Data, bool Hovered);

	UFUNCTION()
		void CloseButtonClick();
	void CloseInvenUI(); //���â���� â ������
	
};

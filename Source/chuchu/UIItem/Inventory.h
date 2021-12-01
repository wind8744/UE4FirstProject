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
 * 1. 인벤토리 ui 클래스
 * 2. 인벤토리 ui안의 표현될 아이템ui 클래스
 * 3. 인벤토리 아이템 ui내부의 진짜 item data
 */
UCLASS()
class CHUCHU_API UInventory : public UUserWidget // 1. 인벤토리 ui 
{
	GENERATED_BODY()

protected:
	// 인벤토리 ui 블프의 타일뷰 츄가한것
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

	class UEquipment* m_Equipclass; //장비창

public:
	void SetEquipClass(UEquipment* _class) { m_Equipclass = _class; }
	UEquipment* GetEquipClass() { return m_Equipclass; }

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

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
	
};

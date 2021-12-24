// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "QuestWidget.h"
#include"../UIItem/Inventory.h"
#include "../UIEquipment/Equipment.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ActorComponent.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "CombineWidget.generated.h"

/**
 *
 */
UCLASS()
class CHUCHU_API UCombineWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UInventory* m_InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UEquipment* m_EquipmentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UQuestWidget* m_QuestWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_CloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_Tap1Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_Tap2Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UWidgetSwitcher* m_WidgetSwitcher;

	UTexture2D* m_SelectedTex;
	UTexture2D* m_ButtonTex;

public:
	UInventory* GetInventoryWidget() { return m_InventoryWidget; }
	UEquipment* GetEquipmentWidget() { return m_EquipmentWidget; }
	UQuestWidget* GetQuestWidget() { return m_QuestWidget; }

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	//widget Drag&drop
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation); //드래그 감지

public:
	UFUNCTION()
		void CloseButtonClick();

	UFUNCTION()
		void Tap1ButtonClick();

	UFUNCTION()
		void Tap2ButtonClick();
};

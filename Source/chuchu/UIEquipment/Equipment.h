// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "EquipTile.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/ActorComponent.h"
#include "Equipment.generated.h"

/**
 * 
 */

UCLASS()
class CHUCHU_API UEquipment : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UEquipTile*> m_EquipTileArry;

	class UInventory* m_Inventoryclass; //�κ��丮

public:
	void InitEquipment();
	void SetInventoryClass(UInventory* _class) { m_Inventoryclass = _class; }
	UInventory* GetInventoryClass() { return m_Inventoryclass; }

	TArray<UEquipTile*> GetEquipTileArray() { return m_EquipTileArry; }

protected:
	virtual void NativeConstruct(); //������
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

};

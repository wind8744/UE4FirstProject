// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Blueprint/UserWidget.h"
#include "EquipTile.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API UEquipTile : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UImage* m_IconTex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBorder* m_EqBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString m_TileName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString m_ItemName; //� �������� ����Ǿ����� �����ϱ� ����, �κ��丮�� �ٽ� �����־�� �ϹǷ�

	
	FString m_ItemOldName;
	bool m_IsEquip;
	class UInventory* m_Inventoryclass; //�κ��丮

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);


public:
	void SetTileName(FString _Name) { m_TileName = _Name; }
	void SetIconTex(UTexture2D* _Tex) { m_IconTex->SetBrushFromTexture(_Tex); }
	void SetIsEquip(bool _Equip) { m_IsEquip = _Equip; }
	void SetInventoryclass(UInventory* _class) { m_Inventoryclass = _class; }
	void SetItemName(FString _Name) 
	{ 
		m_ItemOldName = m_ItemName;
		m_ItemName = _Name;
	}
	bool GetIsEquip() { return m_IsEquip; }
	FString GetItemOldName() const { return m_ItemOldName; }

public:
	UFUNCTION()
		FEventReply OnMouseButtonDoubleClickBorder(FGeometry InMyGeometry, const FPointerEvent& InMouseEvent);

};

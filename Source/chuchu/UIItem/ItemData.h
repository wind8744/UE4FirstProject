// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "ItemData.generated.h"

/**
 * 1. 인벤토리 ui 클래스
 * 2. 인벤토리 ui안의 표현될 아이템ui 클래스
 * 3. 인벤토리 아이템 ui내부의 진짜 item data
 */
UCLASS()
class CHUCHU_API UItemData : public UObject //3. 인벤토리 아이템 ui의 진짜 아이템 데이타
{
	GENERATED_BODY()
public:
	UItemData();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString	m_ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaticMesh* m_PickMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString m_MeshPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTexture2D* m_IconTex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString	m_IconTexPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32	m_Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString m_ItemDesc;

	EItemType m_ItemType;
	EEquipType m_EquipType;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UWorld* m_World;

	class UInventory* OwningInventory;
	
public:

	virtual class UWorld* GetWorld() const { return m_World; }

	void SetInventory(UInventory* _inventory) { OwningInventory = _inventory; }
	
	void SetPickMesh(UStaticMesh* Mesh) { m_PickMesh = Mesh; }

	void SetMeshPath(const FString& EquipmentPath) { m_MeshPath = EquipmentPath; }
	
	void SetItemType(EItemType ItemType, EEquipType EquipTypee){ m_ItemType = ItemType; m_EquipType = EquipTypee; }

	void SetIconTex(UTexture2D* IconTex){ m_IconTex = IconTex; }
	
	void SetNameText(const FString& NameText) { m_ItemName = NameText; }

	void SetItemDesc(const FString& _Des) { m_ItemDesc = _Des; }

	EEquipType GetEquipType() const { return m_EquipType; }

	UTexture2D* GetIconTex()
	{
		return m_IconTex;
	}

	FString GetNameText()
	{
		return m_ItemName;
	}

	FString GetIconPath()
	{
		return m_IconTexPath;
	}
	FString GetItemDesc() { return m_ItemDesc; }

	int32 GetIndex()
	{
		return m_Index;
	}

	void SetIconPath(const FString& Path)
	{
		m_IconTexPath = Path;
	}

	void SetIndex(int32 Index)
	{
		m_Index = Index;
	}

	void Use(class APlayerCharacter* character); //virtual
};

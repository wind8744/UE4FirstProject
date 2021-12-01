// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "MainMenuWidget.generated.h"
/**
 * 
 */
UCLASS()
class CHUCHU_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_InventoryButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_CharacterStateButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_SkillButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_OptionButton;

	class UInventoryList* m_InventoryList;
	class UInventoryTile* m_InventoryTile;
	class UAuctionWidget* m_Auction;

	class UInventory* m_Inventory;
	class UEquipment* m_Equipment;

public:
	void SetInventoryList(class UInventoryList* List)
	{
		m_InventoryList = List;
	}

	void SetInventoryTile(class UInventoryTile* Tile)
	{
		m_InventoryTile = Tile;
	}
		
	void SetAuction(class UAuctionWidget* Auction)
	{
		m_Auction = Auction;
	}

	//
	void SetInventory(class UInventory* Inventory)
	{
		m_Inventory = Inventory;
	}
	void SetEquipment(class UEquipment* Equipment)
	{
		m_Equipment = Equipment;
	}

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UFUNCTION()
		void OnInventory();

	UFUNCTION()
		void OnInventoryTile();

	UFUNCTION()
		void OnAuction();

	UFUNCTION()
		void OnInventory2();

};

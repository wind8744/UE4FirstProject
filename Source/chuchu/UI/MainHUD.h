// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "CharacterHUD.h"
#include "MainMenuWidget.h"
#include "InventoryList.h"
#include "InventoryTile.h"
#include "MainMenuSkillWidget.h"
#include"../UIItem/Inventory.h"
#include "../UIEquipment/Equipment.h"
#include "AuctionWidget.h"
#include "Components/Slider.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCharacterHUD* m_CharacterHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UMainMenuWidget* m_MainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UMainMenuSkillWidget* m_MainMenuSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UAuctionWidget* m_Auction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USlider* m_LandScapeSlider;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UInventory* m_Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UEquipment* m_Equipment;


	class UMaterialParameterCollection* m_LandScapeCollection;
	class UMaterialParameterCollectionInstance* m_LandScapeCollectionInst;

public:
	UCharacterHUD* GetCharacterHUD()
	{
		return m_CharacterHUD;
	}

	UMainMenuWidget* GetMainMenuHUD()
	{
		return m_MainMenu;
	}

	//UInventoryList* GetInventoryList()
	//{
	//	return m_InventoryList;
	//}

	UInventory* GetInventory() { return m_Inventory; }
	UEquipment* GetEquipment() { return m_Equipment; }

	void PopupUI(); //MainPlayerController에서 호출

protected:
	virtual void NativeConstruct(); //생성 ( 소멸도 있음 )
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); //틱마다 들어오는 함수

public:
	UFUNCTION()
		void LandScapeSliderValue(float Value);

};
// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "InventoryList.h"
#include "InventoryTile.h"
#include "../UIItem/Inventory.h"
#include "../UIEquipment/Equipment.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_InventoryButton = Cast<UButton>(GetWidgetFromName(TEXT("InventoryButton"))); //인벤토리버튼
	m_CharacterStateButton = Cast<UButton>(GetWidgetFromName(TEXT("CharacterStateButton")));
	m_SkillButton = Cast<UButton>(GetWidgetFromName(TEXT("SkillButton")));
	m_OptionButton = Cast<UButton>(GetWidgetFromName(TEXT("OptionButton")));

	m_InventoryButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnInventory); //인벤토리 버튼이 눌릴때 onInventory함수가 호출
	m_CharacterStateButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnInventoryTile); //

	// 실제로 쓰는것
	m_SkillButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnAuction);
	m_OptionButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnInventory2);
}

void UMainMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMainMenuWidget::OnInventory() //인벤토리 버튼이 눌릴때 onInventory함수가 호출
{
	if (m_InventoryList->GetVisibility() == ESlateVisibility::Collapsed) //인벤토리가 가려져있을때 호출되면
		m_InventoryList->SetVisibility(ESlateVisibility::SelfHitTestInvisible); //인벤토리가 보이도록

	else
		m_InventoryList->SetVisibility(ESlateVisibility::Collapsed);


	//if (m_Inventory->GetVisibility() == ESlateVisibility::Collapsed) //인벤토리가 가려져있을때 호출되면
	//	m_Inventory->SetVisibility(ESlateVisibility::SelfHitTestInvisible); //인벤토리가 보이도록

	//else
	//	m_Inventory->SetVisibility(ESlateVisibility::Collapsed);
	
}

void UMainMenuWidget::OnInventoryTile()
{
	if (m_InventoryTile->GetVisibility() == ESlateVisibility::Collapsed)
		m_InventoryTile->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	else
		m_InventoryTile->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainMenuWidget::OnAuction()
{
	/*
	if (m_Auction->GetVisibility() == ESlateVisibility::Collapsed)
		m_Auction->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	else
		m_Auction->SetVisibility(ESlateVisibility::Collapsed);
	*/
	
	if (m_Equipment->GetVisibility() == ESlateVisibility::Collapsed)
		m_Equipment->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	else
		m_Equipment->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainMenuWidget::OnInventory2()
{
	if (m_Inventory->GetVisibility() == ESlateVisibility::Collapsed)
		m_Inventory->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	else
		m_Inventory->SetVisibility(ESlateVisibility::Collapsed);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "InventoryList.h"
#include "InventoryTile.h"
#include "../UIItem/Inventory.h"
#include "../UIEquipment/Equipment.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_InventoryButton = Cast<UButton>(GetWidgetFromName(TEXT("InventoryButton"))); //�κ��丮��ư
	m_CharacterStateButton = Cast<UButton>(GetWidgetFromName(TEXT("CharacterStateButton")));
	m_SkillButton = Cast<UButton>(GetWidgetFromName(TEXT("SkillButton")));
	m_OptionButton = Cast<UButton>(GetWidgetFromName(TEXT("OptionButton")));

	m_InventoryButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnInventory); //�κ��丮 ��ư�� ������ onInventory�Լ��� ȣ��
	m_CharacterStateButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnInventoryTile); //

	// ������ ���°�
	m_SkillButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnAuction);
	m_OptionButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnInventory2);
}

void UMainMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMainMenuWidget::OnInventory() //�κ��丮 ��ư�� ������ onInventory�Լ��� ȣ��
{
	if (m_InventoryList->GetVisibility() == ESlateVisibility::Collapsed) //�κ��丮�� ������������ ȣ��Ǹ�
		m_InventoryList->SetVisibility(ESlateVisibility::SelfHitTestInvisible); //�κ��丮�� ���̵���

	else
		m_InventoryList->SetVisibility(ESlateVisibility::Collapsed);


	//if (m_Inventory->GetVisibility() == ESlateVisibility::Collapsed) //�κ��丮�� ������������ ȣ��Ǹ�
	//	m_Inventory->SetVisibility(ESlateVisibility::SelfHitTestInvisible); //�κ��丮�� ���̵���

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
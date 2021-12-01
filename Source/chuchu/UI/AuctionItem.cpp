// Fill out your copyright notice in the Description page of Project Settings.


#include "AuctionItem.h"
#include "AuctionItemData.h"
#include "Components/CanvasPanelSlot.h"

void UAuctionItem::Selection()
{
	m_Selection->SetOpacity(0.3f); //선택된 아이템
}

void UAuctionItem::UnSelection()
{
	m_Selection->SetOpacity(0.f);
}

void UAuctionItem::NativeConstruct()
{
	Super::NativeConstruct();

	m_NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameText")));
	m_Selection = Cast<UImage>(GetWidgetFromName(TEXT("Selection")));
}

void UAuctionItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

//	auctionWidget.cpp에서 m_Menu->AddItem(Data); //setdata함수를 연결했기 때문에 auctionitem::setdata 함수가 호출된다.
void UAuctionItem::SetData(UObject* Data)
{
	UAuctionItemData* Item = Cast<UAuctionItemData>(Data);

	Item->SetLinkItem(this);//아이템 등록

	m_NameText->SetText(FText::FromString(Item->GetNameText()));

	//네임텍의 슬롯을 가지고 옴
	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(m_NameText->Slot);

	if (PanelSlot)
		PanelSlot->SetPosition(FVector2D(Item->GetTreeLevel() * 50.f, 0.f));

	PanelSlot = Cast<UCanvasPanelSlot>(m_Selection->Slot);

	if (PanelSlot)
		PanelSlot->SetPosition(FVector2D(Item->GetTreeLevel() * 50.f, 0.f));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AuctionWidget.h"
#include "../chuchuGameInstance.h"
#include "AuctionItemData.h"

void UAuctionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_SelectItem = nullptr;

	m_Menu = Cast<UTreeView>(GetWidgetFromName(TEXT("Tree")));

	m_Menu->OnItemDoubleClicked().AddUObject(this, &UAuctionWidget::ItemDoubleClick);
	m_Menu->OnItemIsHoveredChanged().AddUObject(this, &UAuctionWidget::ItemHOvered);

	/* 
	*auctionWidget 블프에서  Tree의 디테일에서
	*이벤트 보면 OnGetItemChildren 함수가 있는데
	*C++ 코드에서 부를 수 있는데 부르면 트리 형태로 밑에 딸린 자식들
	*다 가져온다.
	*/
	m_Menu->SetOnGetItemChildren(this, &UAuctionWidget::GetItemChildren);

	UAuctionItemData* Data = NewObject<UAuctionItemData>(this,UAuctionItemData::StaticClass());
	Data->SetNameText(TEXT("무기"));

	m_Menu->AddItem(Data); //setdata함수를 연결했기 때문에 auctionitem::setdata 함수가 호출된다.

	Data->CreateChildren(TEXT("검"));
	Data->CreateChildren(TEXT("활"));
	Data->CreateChildren(TEXT("지팡이"));
	Data->CreateChildren(TEXT("총"));

	Data = NewObject<UAuctionItemData>(this,UAuctionItemData::StaticClass());
	Data->SetNameText(TEXT("방어구"));

	Data->CreateChildren(TEXT("갑옷"));
	Data->CreateChildren(TEXT("투구"));
	Data->CreateChildren(TEXT("장갑"));
	Data->CreateChildren(TEXT("신발"));

	m_Menu->AddItem(Data);

	Data = NewObject<UAuctionItemData>(this,UAuctionItemData::StaticClass());

	Data->SetNameText(TEXT("포션"));

	Data->CreateChildren(TEXT("체력"));
	Data->CreateChildren(TEXT("마나"));

	m_Menu->AddItem(Data);
}

void UAuctionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

FReply UAuctionWidget::NativeOnMouseMove(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	return FReply::Handled();
}

void UAuctionWidget::ItemDoubleClick(UObject* Data)
{
	if (m_SelectItem)
		m_SelectItem->UnSelection();

	m_SelectItem = Cast<UAuctionItemData>(Data);
}

void UAuctionWidget::ItemHOvered(UObject* Data, bool Hovered)
{
	UAuctionItemData* Item = Cast<UAuctionItemData>(Data);

	if (Hovered)
	{
		Item->Selection();
	}

	else if (m_SelectItem != Item)
	{
		Item->UnSelection();
	}
}

//차일드가 추가될 때 들어오는 곳
void UAuctionWidget::GetItemChildren(UObject* Data,TArray<UObject*>& ItemArray)
{
	UAuctionItemData* Item = Cast<UAuctionItemData>(Data);

	if (Item)
		ItemArray = Item->GetChildArray(); //내부적으로 가지고있는 차일드를 받는다
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemList.h"
#include "InventoryItemDataList.h"

void UInventoryItemList::NativeConstruct()
{
	Super::NativeConstruct();

	m_IconImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemIcon")));
	m_NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameText")));
}

void UInventoryItemList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryItemList::SetData(UObject* Data)
{
	/*
	* #42번영상
	inventorylist.cpp 에서
	AddItem 함수가 불릴 때 아까 만들어놓았던
		  
		//	이벤트에서 on list item object 이벤트를 추가할 수 있는데
		//	리스트에서 아이템이 추가가 될 때 이 인터페이스가 자동으로 호출되게 만들어준다.
		//	블푸에 추가함
		//
		// On List Item Object Set 이벤트 함수가 호출된다.

	블프에 만들어 놓긴 했지만 원래는 cpp 에서도 호출할 수 있다.
	선생님은 이 이벤트에 setdata함수를 연결(블프에서)해서
	setdata함수가 cpp에서 불리도록 할 것임

	cpp에서 additem함수가 실행이 될 때
	on List item object set이벤트 함수가 호출이 될 것이고
	List item object로 넣은 아이템 정보가 들어옴
	이 데이터는 연결된 setdata함수로 data 변수로 들어올것임
	*/

	UInventoryItemDataList* InvenData = Cast<UInventoryItemDataList>(Data);

	m_NameText->SetText(FText::FromString(InvenData->GetNameText())); //이름 넣고

	UTexture2D* IconTexture = LoadObject<UTexture2D>(nullptr,
		*InvenData->GetIconPath());

	if (IconTexture)
		m_IconImage->SetBrushFromTexture(IconTexture);  //이미지 넣고
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryList.h"
#include "InventoryItemDataList.h"
#include "../chuchuGameInstance.h"

void UInventoryList::NativeConstruct()
{
	Super::NativeConstruct();

	m_InventoryList = Cast<UListView>(GetWidgetFromName(TEXT("InventoryList")));

	//키값
	/*
	FString	ItemNameArray[3] =
	{
		TEXT("BF대검"),
		TEXT("칠흑의양날도끼"),
		TEXT("HP포션")
	};

	UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());
	
	for (int32 i = 0; i < 100; ++i)
	{
		int32	Index = FMath::RandRange(0, 2); //0 ~ 2사이 랜덤값

		const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(ItemNameArray[Index]);  //랜덤하게 info가져옴

		UInventoryItemDataList* Data = NewObject<UInventoryItemDataList>(this,
			UInventoryItemDataList::StaticClass());

		Data->SetNameText(Info->Name);
		Data->SetIconPath(Info->IconPath);
		Data->SetIndex(i);

		m_InventoryList->AddItem(Data); // 아이템을 추가하고싶다 additem 지우고싶다 remove 

		// AddItem 함수가 불릴 때 아까 만들어놓았던
		//  
		//	이벤트에서 on list item object 이벤트를 추가할 수 있는데
		//	리스트에서 아이템이 추가가 될 때 이 인터페이스가 자동으로 호출되게 만들어준다.
		//	블푸에 추가함
		//
		// On List Item Object Set 이벤트 함수가 호출된다.
	}
	*/
	//블루프린트에서도 부를 수 있고 cpp에서도 이런식으로 이벤트 함수로 부를수도 있다.
	//인벤토리가 눌렸을때 불리는 함수 
	m_InventoryList->OnItemClicked().AddUObject(this, &UInventoryList::ItemClick);
	//많이 쓰이는 함수들 
	//m_InventoryList->OnItemSelectionChanged();
	//m_InventoryList->OnItemDoubleClicked()  //더블클릭시
	//m_InventoryList->OnItemIsHoveredChanged() //
}

void UInventoryList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryList::ItemClick(UObject* Data)
{
	UInventoryItemDataList* Item = Cast<UInventoryItemDataList>(Data);

	if (Item)
	{
		m_InventoryList->RemoveItem(Data); //아이템 제거
		PrintViewport(1.f, FColor::Red, Item->GetNameText());
	}
}

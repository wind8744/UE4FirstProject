 //Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTile.h"
#include "../chuchuGameInstance.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryItemDataTile.h"

void UInventoryTile::NativeConstruct()
{
	Super::NativeConstruct();

	m_ItemCount = 0;

	m_InventoryTile = Cast<UTileView>(GetWidgetFromName(TEXT("InventoryTile")));
	m_InventorySlot = Cast<UTileView>(GetWidgetFromName(TEXT("InventorySlot")));
	m_ItemDescWidget = Cast<UItemDescWidget>(GetWidgetFromName(TEXT("UI_ItemDesc")));

	m_InventoryTile->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	m_InventorySlot->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());

	FString	ItemNameArray[3] =
	{
		TEXT("BF대검"),
		TEXT("칠흑의양날도끼"),
		TEXT("HP포션")
	};

	// 슬롯은 무조건 20개를 유지한다.
	for (int32 i = 0; i < 20; ++i)
	{
		UObject* Data = NewObject<UObject>(this, UObject::StaticClass());

		m_InventorySlot->AddItem(Data);

		m_SlotArray.Add(Data);
	}


	
	/*
	* 유저위셋이 위젯트리라는 멤버변수를 갖고 있음
	* 위젯트리란 블프에서 계층구조 전체를 말하는거 
	* ConstructWidget : 위젯을 여기서 만들어서 캔버스에 추가할 수 잇다.
	* 
	UImage* Item0 = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Item0")); //이미지 위젯을 하나 추가

	UCanvasPanel* Canvas = WidgetTree->FindWidget<UCanvasPanel>(TEXT("CanvasPanel1")); //캔버스에 추가할 수 잇다. , get widget from name 하뭇도 사용 가능

	UCanvasPanelSlot* PanelSlot = Canvas->AddChildToCanvas(Item0);

	PanelSlot->SetPosition(FVector2D(200.f, 50.f)); //위치 사이즈 조절
	PanelSlot->SetSize(FVector2D(50.f, 50.f));*/


	/*
	for (int32 i = 0; i < 100; ++i)
	{
		int32	Index = FMath::RandRange(0, 2);

		const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(ItemNameArray[Index]);

		UInventoryItemDataTile* Data = NewObject<UInventoryItemDataTile>(this,
			UInventoryItemDataTile::StaticClass());

		Data->SetNameText(Info->Name);
		Data->SetIconPath(Info->IconPath);
		Data->SetIndex(i);

		m_InventoryTile->AddItem(Data);

		if (i >= 20)//아이템이 20 이상이 되면  슬롯을 하나 더 추가해줌 
		{
			UObject* Data1 = NewObject<UObject>(this, UObject::StaticClass());

			m_InventorySlot->AddItem(Data1); //슬롯이 추가되도록

			m_SlotArray.Add(Data1);
		}
	}*/

	m_ItemCount = 100;
	m_MouseHovered = false;

	//m_InventoryTile->SetScrollOffset(10.f); //스크롤을 강제로 처리

	m_InventoryTile->OnItemClicked().AddUObject(this, &UInventoryTile::ItemClick);
	//m_InventoryTile->OnItemScrolledIntoView().AddUObject(this, &UInventoryTile::ItemScroll);
	//m_InventoryTile->OnItemSelectionChanged()
	//m_InventoryTile->OnItemDoubleClicked()
	m_InventoryTile->OnItemIsHoveredChanged().AddUObject(this, &UInventoryTile::ItemHOvered); //아이템설명창 띄우고싶을때 필요
}

void UInventoryTile::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//if (m_MouseHovered)
	//{
	//	//m_ItemDescWidget->SetPositionInViewport(FVector2D(MouseX, MouseY));
	//	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(m_ItemDescWidget->Slot);

	//	if (PanelSlot)
	//	{
	//		float	MouseX = 0.f, MouseY = 0.f;
	//		if (GetWorld()->GetFirstPlayerController()->GetMousePosition(MouseX, MouseY))
	//		{
	//			//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("X : %.5f Y : %.5f"), MouseX, MouseY));
	//		}

	//		//USlateBlueprintLibrary::ScreenToWidgetLocal

	//		PanelSlot->SetPosition(FVector2D(MouseX, MouseY));
	//	}
	//}
}

// 마우스가 온 상태일때만
FReply UInventoryTile::NativeOnMouseMove(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	/*
	* GetWorld()->GetFirstPlayerController()->GetMousePosition(); //마우스 위치값을 얻어올 숭 ㅣㅆ따.
	*/

	if (m_MouseHovered)
	{
		//Vector2D	S

		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(m_ItemDescWidget->Slot); //슬롯을 패널로 변환

		if (PanelSlot)
		{
			FVector2D	WidgetPos = InMouseEvent.GetScreenSpacePosition();

			//USlateBlueprintLibrary::ScreenToWidgetLocal(this, InGeometry,
			//	InMouseEvent.GetScreenSpacePosition(), WidgetPos);

			// 마우스의 Screen좌표를 위젯의 Local 좌표로 변경한다.
			WidgetPos = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry,
				WidgetPos);

			WidgetPos.X += 1.f;
			WidgetPos.Y += 1.f;

			PanelSlot->SetPosition(WidgetPos); //마우스위치를 반대로세팅

			//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("X : %.5f Y : %.5f"), WidgetPos.X, WidgetPos.Y));
		}
	}

	return FReply::Handled();
}

void UInventoryTile::ItemClick(UObject* Data)
{
	UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(Data);

	if (Item)
	{
		m_InventoryTile->RemoveItem(Data);
		m_InventorySlot->RemoveItem(m_SlotArray[Item->GetIndex()]); //아이템 삭제하면 슬롯에서도 삭제

		if (m_SlotArray.Num() > 20) //슬롯 20개 넘어가면 (어짜피안보이므로) 삭제
			m_SlotArray.RemoveAt(Item->GetIndex());
	}
}

void UInventoryTile::ItemScroll(UObject* Data, UUserWidget* widget)
{
}

void UInventoryTile::ItemHOvered(UObject* Data, bool Hovered)
{
	m_MouseHovered = Hovered;

	if (Hovered)
	{
		m_ItemDescWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(Data);

		if (Item)
		{
			m_ItemDescWidget->SetNameText(Item->GetNameText());
			m_ItemDescWidget->SetIconTexture(Item->GetIconTexture());
			//PrintViewport(1.f, FColor::Red, Item->GetNameText());
		}
	}

	else
	m_ItemDescWidget->SetVisibility(ESlateVisibility::Collapsed);

}


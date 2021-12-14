// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment.h"
#include "../UIItem/Inventory.h"
#include "Components/CanvasPanelSlot.h"
#include "../Player/MainPlayerController.h"
#include "../chuchuGameModeBase.h"

void UEquipment::NativeConstruct()
{
	Super::NativeConstruct();

	//m_EquipmentTile = Cast<UEquipTile>(GetWidgetFromName(TEXT("UI_EquipTile1")));
	UEquipTile* EqTile;
	FString firText = "UI_EquipTile"; 
	for (int i = 1; i < 7; i++)
	{
		FString secText = firText + FString::FromInt(i);
		EqTile = Cast<UEquipTile>(GetWidgetFromName(*secText));
		EqTile->SetTileName(GetEnumToString((EEquipType)(i-1)));
		EqTile->SetInventoryclass(m_Inventoryclass);
		m_EquipTileArry.Add(EqTile);
	}

	m_CloseButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_55")));
	m_CloseButton->OnClicked.AddDynamic(this, &UEquipment::CloseButtonClick);
}

void UEquipment::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

// 마우스가 온 상태일때만
//FReply UEquipment::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	Super::NativeOnMouseMove(InGeometry, InMouseEvent);
//
//	return FReply::Handled();
//}

void UEquipment::CloseButtonClick()
{
	if (GetVisibility() == ESlateVisibility::Collapsed) //인벤토리가 가려져있을때 호출되면
		SetVisibility(ESlateVisibility::SelfHitTestInvisible); //인벤토리가 보이도록

	else
	{
		//SetVisibility(ESlateVisibility::Collapsed);
		AchuchuGameModeBase* chuMode = Cast<AchuchuGameModeBase>(GetWorld()->GetAuthGameMode());
		chuMode->GetMainHUD()->CloseAllUI();
		//m_Inventoryclass->CloseInvenUI();
	}
}

void UEquipment::CloseEquipUI()
{
	if (GetVisibility() == ESlateVisibility::Collapsed)
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UEquipment::InitEquipment()
{
	int32 Arraysize = m_EquipTileArry.Num();
	for (int i = 0; i < Arraysize; i++)
	{
		m_EquipTileArry[i]->SetInventoryclass(m_Inventoryclass);
	}
}


#include "Blueprint/WidgetBlueprintLibrary.h"
FReply UEquipment::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Allows users to handle events and return information to the underlying UI layer.
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, FKey("LeftMouseButton")); //왼쪽 마우스를 누르면 이벤트가 들어옴
	//DetectDragIfPressed 이; 함수를 통해 밑의 NativeOnDragDetected함수가 호출됨

	return reply.NativeReply;
}

#include "../UI/WidgetDragDropOperation.h"
void UEquipment::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UWidgetDragDropOperation* Wigddo = Cast<UWidgetDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UWidgetDragDropOperation::StaticClass()));
	//내가 만든 드래그드랍 클래스 갖고옴

	if (nullptr == Wigddo)
		return;

	Wigddo->WidgetTodrag = this;
	//옮길 위젯 (멤버변수로 저장)

	Wigddo->MouseOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	//마우스 offset (멤버변수에 저장)

	Wigddo->DefaultDragVisual = this;
	//드래그 할 때 보일 위젯 

	Wigddo->Pivot = EDragPivot::MouseDown;
	//마우스 다운을 한 지점을 기준으로

	OutOperation = Wigddo;
	//outoperation의 참ㅁ조가 널이 아니라면 native on drop이 호출

	this->RemoveFromParent(); //드래그 할 때 그 전에있던것은 삭제 
}

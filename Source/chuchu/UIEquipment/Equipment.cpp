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

// ���콺�� �� �����϶���
//FReply UEquipment::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	Super::NativeOnMouseMove(InGeometry, InMouseEvent);
//
//	return FReply::Handled();
//}

void UEquipment::CloseButtonClick()
{
	if (GetVisibility() == ESlateVisibility::Collapsed) //�κ��丮�� ������������ ȣ��Ǹ�
		SetVisibility(ESlateVisibility::SelfHitTestInvisible); //�κ��丮�� ���̵���

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
	reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, FKey("LeftMouseButton")); //���� ���콺�� ������ �̺�Ʈ�� ����
	//DetectDragIfPressed ��; �Լ��� ���� ���� NativeOnDragDetected�Լ��� ȣ���

	return reply.NativeReply;
}

#include "../UI/WidgetDragDropOperation.h"
void UEquipment::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UWidgetDragDropOperation* Wigddo = Cast<UWidgetDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UWidgetDragDropOperation::StaticClass()));
	//���� ���� �巡�׵�� Ŭ���� �����

	if (nullptr == Wigddo)
		return;

	Wigddo->WidgetTodrag = this;
	//�ű� ���� (��������� ����)

	Wigddo->MouseOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	//���콺 offset (��������� ����)

	Wigddo->DefaultDragVisual = this;
	//�巡�� �� �� ���� ���� 

	Wigddo->Pivot = EDragPivot::MouseDown;
	//���콺 �ٿ��� �� ������ ��������

	OutOperation = Wigddo;
	//outoperation�� �������� ���� �ƴ϶�� native on drop�� ȣ��

	this->RemoveFromParent(); //�巡�� �� �� �� �����ִ����� ���� 
}

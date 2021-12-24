// Fill out your copyright notice in the Description page of Project Settings.


#include "CombineWidget.h"
#include "../chuchuGameModeBase.h"
#include "../chuchuGameInstance.h"

void UCombineWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_InventoryWidget = Cast<UInventory>(GetWidgetFromName(TEXT("UI_Inventory1")));
	m_EquipmentWidget = Cast<UEquipment>(GetWidgetFromName(TEXT("UI_Equipment1")));
	m_QuestWidget = Cast<UQuestWidget>(GetWidgetFromName(TEXT("UI_QUest1")));
	m_CloseButton = Cast<UButton>(GetWidgetFromName(TEXT("CloseButton1")));
	m_Tap1Button = Cast<UButton>(GetWidgetFromName(TEXT("tap1")));
	m_Tap2Button = Cast<UButton>(GetWidgetFromName(TEXT("tap2")));
	m_WidgetSwitcher = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("WidgetSwitcher_59")));

	m_CloseButton->OnClicked.AddDynamic(this, &UCombineWidget::CloseButtonClick);
	m_Tap1Button->OnClicked.AddDynamic(this, &UCombineWidget::Tap1ButtonClick);
	m_Tap2Button->OnClicked.AddDynamic(this, &UCombineWidget::Tap2ButtonClick);

	//m_ButtonTex = LoadObject<UTexture2D>(nullptr, (TEXT("Texture2D'/Game/Classic_RPG_GUI/Parts/longbutton_frame1.longbutton_frame'")));
	//m_SelectedTex = LoadObject<UTexture2D>(nullptr, (TEXT("Texture2D'/Game/Classic_RPG_GUI/Parts/longbutton_frame11.longbutton_frame11'")));
}

void UCombineWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCombineWidget::Tap1ButtonClick()
{
	m_WidgetSwitcher->SetActiveWidgetIndex(0);
	m_ButtonTex = LoadObject<UTexture2D>(nullptr, (TEXT("Texture2D'/Game/Classic_RPG_GUI/Parts/longbutton_frame.longbutton_frame'")));
	m_SelectedTex = LoadObject<UTexture2D>(nullptr, (TEXT("Texture2D'/Game/Classic_RPG_GUI/Parts/longbutton_frame11.longbutton_frame11'")));
	if (m_SelectedTex && m_ButtonTex) //Ÿ�Ͽ� ���� ������ �̹��� ���̱�
	{
		FButtonStyle test1;
		FSlateBrush brush1;
		brush1.SetResourceObject(m_SelectedTex);
		test1.SetNormal(brush1);
		m_Tap1Button->SetStyle(test1);

		brush1.SetResourceObject(m_ButtonTex);
		test1.SetNormal(brush1);
		m_Tap2Button->SetStyle(test1);
	}
}
void UCombineWidget::Tap2ButtonClick()
{
	m_WidgetSwitcher->SetActiveWidgetIndex(1);
	m_ButtonTex = LoadObject<UTexture2D>(nullptr, (TEXT("Texture2D'/Game/Classic_RPG_GUI/Parts/longbutton_frame.longbutton_frame'")));
	m_SelectedTex = LoadObject<UTexture2D>(nullptr, (TEXT("Texture2D'/Game/Classic_RPG_GUI/Parts/longbutton_frame11.longbutton_frame11'")));
	if (m_SelectedTex && m_ButtonTex)
	{
		FButtonStyle test1;
		FSlateBrush brush1;
		brush1.SetResourceObject(m_SelectedTex);
		test1.SetNormal(brush1);
		m_Tap2Button->SetStyle(test1);

		brush1.SetResourceObject(m_ButtonTex);
		test1.SetNormal(brush1);
		m_Tap1Button->SetStyle(test1);
	}
}



void UCombineWidget::CloseButtonClick()
{
	if (GetVisibility() == ESlateVisibility::Collapsed) //�κ��丮�� ������������ ȣ��Ǹ�
		SetVisibility(ESlateVisibility::SelfHitTestInvisible); //�κ��丮�� ���̵���

	else
	{
		AchuchuGameModeBase* chuMode = Cast<AchuchuGameModeBase>(GetWorld()->GetAuthGameMode());
		chuMode->GetMainHUD()->CloseAllUI();
	}
}

#include "Blueprint/WidgetBlueprintLibrary.h"
FReply UCombineWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Allows users to handle events and return information to the underlying UI layer.
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, FKey("LeftMouseButton")); //���� ���콺�� ������ �̺�Ʈ�� ����
	//DetectDragIfPressed ��; �Լ��� ���� ���� NativeOnDragDetected�Լ��� ȣ���

	return reply.NativeReply;
}

#include "../UI/WidgetDragDropOperation.h"
void UCombineWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
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
	//outoperation�� ������ ���� �ƴ϶�� native on drop�� ȣ��

	this->RemoveFromParent(); //�巡�� �� �� �� �����ִ����� ���� 
}

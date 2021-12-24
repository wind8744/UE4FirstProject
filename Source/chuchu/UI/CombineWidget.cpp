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
	if (m_SelectedTex && m_ButtonTex) //타일에 들어온 아이템 이미지 붙이기
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
	if (GetVisibility() == ESlateVisibility::Collapsed) //인벤토리가 가려져있을때 호출되면
		SetVisibility(ESlateVisibility::SelfHitTestInvisible); //인벤토리가 보이도록

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
	reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, FKey("LeftMouseButton")); //왼쪽 마우스를 누르면 이벤트가 들어옴
	//DetectDragIfPressed 이; 함수를 통해 밑의 NativeOnDragDetected함수가 호출됨

	return reply.NativeReply;
}

#include "../UI/WidgetDragDropOperation.h"
void UCombineWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
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
	//outoperation의 참조가 널이 아니라면 native on drop이 호출

	this->RemoveFromParent(); //드래그 할 때 그 전에있던것은 삭제 
}

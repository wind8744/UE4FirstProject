// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"
#include "../Player/MainPlayerController.h"

void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	// 변수인지가 안되엉 있어야 여기서 가져다가 쓸 수 있다....
	// 이름으로 변수를 얻어올 수 있다.
	m_CharacterHUD = Cast<UCharacterHUD>(GetWidgetFromName(TEXT("UI_CharacterHUD")));
	m_MainMenuSkill = Cast<UMainMenuSkillWidget>(GetWidgetFromName(TEXT("UI_MainMenuSkill")));
	m_MainMenu = Cast<UMainMenuWidget>(GetWidgetFromName(TEXT("UI_MainMenu")));
	m_CombineWidget = Cast<UCombineWidget>(GetWidgetFromName(TEXT("UI_Combine1")));

	//for drap &drop 상호작용을 위해
	Visibility = ESlateVisibility::Visible;
	m_CombineWidget->Visibility = ESlateVisibility::Collapsed;

	m_Inventory = m_CombineWidget->GetInventoryWidget();
	m_Equipment = m_CombineWidget->GetEquipmentWidget();
	m_QuestWidget = m_CombineWidget->GetQuestWidget();

	m_MainMenu->SetUIWidget(m_CombineWidget);

	//장비창과 인벤토리에 서로의 주소값 세팅
	m_Inventory->SetEquipClass(m_Equipment);
	m_Equipment->SetInventoryClass(m_Inventory);
	m_Equipment->InitEquipment();
}

void UMainHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

#include "../UI/WidgetDragDropOperation.h"
//widget drop (main hud에서 가지고있는 위젯들이) 마우스를 떼었을 경우 들어오는 함수
bool UMainHUD::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	// native on drag detected에서 반환된 udragdropoperation정보가 여기로 들어오게 된다
	// 하지만 inoperation가 nullptr 이라면 nativeondrop은 호출되지 않는다

	UWidgetDragDropOperation* wigdd = Cast<UWidgetDragDropOperation>(InOperation);
	if (nullptr == wigdd) 
		return false;

	wigdd->WidgetTodrag->AddToViewport();

	//드래그할 위젯의 위치를 마우스 위치로
	FVector2D NewPosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition()) - wigdd->MouseOffset;
	UUserWidget* widget = wigdd->WidgetTodrag;
	widget->SetPositionInViewport(NewPosition, false);
	return true;
}

// 메인 플레이어 컨트롤 클래스에서 ui 키를 눌렀을 때 불리는 함수
void UMainHUD::PopupUI()
{
	if(m_CombineWidget->GetVisibility() == ESlateVisibility::Collapsed)
		m_CombineWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	else
		m_CombineWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainHUD::CloseAllUI()
{
	m_CombineWidget->SetVisibility(ESlateVisibility::Collapsed);

	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	PController->SetInputMode(FInputModeGameOnly()); // 커서 없어지고 마우스 방향으로 카메라 회전
	PController->bShowMouseCursor = false;
}
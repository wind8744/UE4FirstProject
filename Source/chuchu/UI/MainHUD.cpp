// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"
#include "../Player/MainPlayerController.h"

void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	// ���������� �ȵǾ� �־�� ���⼭ �����ٰ� �� �� �ִ�....
	// �̸����� ������ ���� �� �ִ�.
	m_CharacterHUD = Cast<UCharacterHUD>(GetWidgetFromName(TEXT("UI_CharacterHUD")));
	m_MainMenuSkill = Cast<UMainMenuSkillWidget>(GetWidgetFromName(TEXT("UI_MainMenuSkill")));
	m_MainMenu = Cast<UMainMenuWidget>(GetWidgetFromName(TEXT("UI_MainMenu")));
	m_CombineWidget = Cast<UCombineWidget>(GetWidgetFromName(TEXT("UI_Combine1")));

	//for drap &drop ��ȣ�ۿ��� ����
	Visibility = ESlateVisibility::Visible;
	m_CombineWidget->Visibility = ESlateVisibility::Collapsed;

	m_Inventory = m_CombineWidget->GetInventoryWidget();
	m_Equipment = m_CombineWidget->GetEquipmentWidget();
	m_QuestWidget = m_CombineWidget->GetQuestWidget();

	m_MainMenu->SetUIWidget(m_CombineWidget);

	//���â�� �κ��丮�� ������ �ּҰ� ����
	m_Inventory->SetEquipClass(m_Equipment);
	m_Equipment->SetInventoryClass(m_Inventory);
	m_Equipment->InitEquipment();
}

void UMainHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

#include "../UI/WidgetDragDropOperation.h"
//widget drop (main hud���� �������ִ� ��������) ���콺�� ������ ��� ������ �Լ�
bool UMainHUD::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	// native on drag detected���� ��ȯ�� udragdropoperation������ ����� ������ �ȴ�
	// ������ inoperation�� nullptr �̶�� nativeondrop�� ȣ����� �ʴ´�

	UWidgetDragDropOperation* wigdd = Cast<UWidgetDragDropOperation>(InOperation);
	if (nullptr == wigdd) 
		return false;

	wigdd->WidgetTodrag->AddToViewport();

	//�巡���� ������ ��ġ�� ���콺 ��ġ��
	FVector2D NewPosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition()) - wigdd->MouseOffset;
	UUserWidget* widget = wigdd->WidgetTodrag;
	widget->SetPositionInViewport(NewPosition, false);
	return true;
}

// ���� �÷��̾� ��Ʈ�� Ŭ�������� ui Ű�� ������ �� �Ҹ��� �Լ�
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
	PController->SetInputMode(FInputModeGameOnly()); // Ŀ�� �������� ���콺 �������� ī�޶� ȸ��
	PController->bShowMouseCursor = false;
}
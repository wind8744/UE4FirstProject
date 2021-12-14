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
	m_Inventory = Cast<UInventory>(GetWidgetFromName(TEXT("UI_Inventory")));
	m_Equipment = Cast<UEquipment>(GetWidgetFromName(TEXT("UI_Equipment")));
	m_QuestWidget = Cast<UQuestWidget>(GetWidgetFromName(TEXT("UI_QUest")));
	
	//test
	m_Auction = Cast<UAuctionWidget>(GetWidgetFromName(TEXT("UI_AuctionWidget")));
	m_LandScapeSlider = Cast<USlider>(GetWidgetFromName(TEXT("LandScapeTiling")));

	//MainHUD �������Ʈ���� LandScapeTiling�� �������� üũ ���� �� �� �����Ͽ��� �̺�Ʈ �Լ� �̸����� Ȯ���� �� �ִ�.
	//�Լ� �̸� Ȯ�� �� �ٽ� �������� üũ �� �����Ҵ� �Ͽ� ����ϸ� ��
	//�� �ٲ𶧸��� �Լ��� ȣ����
	m_LandScapeSlider->OnValueChanged.AddDynamic(this, &UMainHUD::LandScapeSliderValue);

	m_LandScapeCollection = LoadObject<UMaterialParameterCollection>(GetWorld(), TEXT("MaterialParameterCollection'/Game/LandScape/MCMainLandScapeData.MCMainLandScapeData'"));

	//�ν��Ͻ� ����� ��� , �ν��Ͻ� ��ü�� �ϳ� �������, ������ ���� ��Ʈ�� �ϴ� ���� �ν���Ʈ�� ����
	m_LandScapeCollectionInst = GetWorld()->GetParameterCollectionInstance(m_LandScapeCollection);

	m_MainMenu->SetAuction(m_Auction);
	m_MainMenu->SetInventory(m_Inventory);
	m_MainMenu->SetEquipment(m_Equipment);
	m_MainMenu->SetQuestWidget(m_QuestWidget);

	/*m_MainMenu->SetInventoryButtonClickCallback<UMainHUD>(this,
		&UMainHUD::InventoryButtonClick);*/

	//���â�� �κ��丮�� ������ �ּҰ� ����
	m_Inventory->SetEquipClass(m_Equipment);
	m_Equipment->SetInventoryClass(m_Inventory);
	m_Equipment->InitEquipment();

	//for drap &drop ��ȣ�ۿ��� ����
	Visibility = ESlateVisibility::Visible;
}

void UMainHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMainHUD::LandScapeSliderValue(float Value)
{
	//����� �Ǵ°��� �ƴ��� �Ұ����� �������� 
	//���� ��(�����̴��� ������ ��)�� ���彺ĳ���� ����Ÿ�� Ÿ�ϸ� data���� set
	bool Find = m_LandScapeCollectionInst->SetScalarParameterValue(TEXT("Tiling"), Value);
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
	if (m_Equipment->GetVisibility() == ESlateVisibility::Collapsed)
		m_Equipment->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	else
		m_Equipment->SetVisibility(ESlateVisibility::Collapsed);


	if (m_Inventory->GetVisibility() == ESlateVisibility::Collapsed)
		m_Inventory->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	else
		m_Inventory->SetVisibility(ESlateVisibility::Collapsed);

	if (m_QuestWidget->GetVisibility() == ESlateVisibility::Collapsed)
		m_QuestWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	else
		m_QuestWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainHUD::CloseAllUI()
{

	m_Equipment->SetVisibility(ESlateVisibility::Collapsed);
	m_Inventory->SetVisibility(ESlateVisibility::Collapsed);
	m_QuestWidget->SetVisibility(ESlateVisibility::Collapsed);

	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	PController->SetInputMode(FInputModeGameOnly()); // Ŀ�� �������� ���콺 �������� ī�޶� ȸ��
	PController->bShowMouseCursor = false;

	//if (m_Inventory->GetVisibility() == ESlateVisibility::Collapsed)
	//	m_Inventory->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	//else
	//	m_Inventory->SetVisibility(ESlateVisibility::Collapsed);

	//if (m_QuestWidget->GetVisibility() == ESlateVisibility::Collapsed)
	//	m_QuestWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	//else
	//	m_QuestWidget->SetVisibility(ESlateVisibility::Collapsed);

}
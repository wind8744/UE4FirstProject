// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"


void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	// ���������� �ȵǾ� �־�� ���⼭ �����ٰ� �� �� �ִ�....
	// �̸����� ������ ���� �� �ִ�.
	m_CharacterHUD = Cast<UCharacterHUD>(GetWidgetFromName(TEXT("UI_CharacterHUD")));
	m_MainMenu = Cast<UMainMenuWidget>(GetWidgetFromName(TEXT("UI_MainMenu")));
	m_Inventory = Cast<UInventory>(GetWidgetFromName(TEXT("UI_Inventory")));
	m_Equipment = Cast<UEquipment>(GetWidgetFromName(TEXT("UI_Equipment")));

	//���͵�
	m_InventoryList = Cast<UInventoryList>(GetWidgetFromName(TEXT("UI_InventoryList")));
	m_InventoryTile = Cast<UInventoryTile>(GetWidgetFromName(TEXT("UI_InventoryTile")));
	m_Auction = Cast<UAuctionWidget>(GetWidgetFromName(TEXT("UI_AuctionWidget")));
	m_LandScapeSlider = Cast<USlider>(GetWidgetFromName(TEXT("LandScapeTiling")));

	//MainHUD �������Ʈ���� LandScapeTiling�� �������� üũ ���� �� �� �����Ͽ��� �̺�Ʈ �Լ� �̸����� Ȯ���� �� �ִ�.
	//�Լ� �̸� Ȯ�� �� �ٽ� �������� üũ �� �����Ҵ� �Ͽ� ����ϸ� ��
	//�� �ٲ𶧸��� �Լ��� ȣ����
	m_LandScapeSlider->OnValueChanged.AddDynamic(this, &UMainHUD::LandScapeSliderValue);


	m_LandScapeCollection = LoadObject<UMaterialParameterCollection>(GetWorld(),
		TEXT("MaterialParameterCollection'/Game/LandScape/MCMainLandScapeData.MCMainLandScapeData'"));

	//�ν��Ͻ� ����� ��� , �ν��Ͻ� ��ü�� �ϳ� �������, ������ ���� ��Ʈ�� �ϴ� ���� �ν���Ʈ�� ����
	m_LandScapeCollectionInst = GetWorld()->GetParameterCollectionInstance(m_LandScapeCollection);

	m_MainMenu->SetInventoryList(m_InventoryList);
	m_MainMenu->SetInventoryTile(m_InventoryTile);
	m_MainMenu->SetAuction(m_Auction);

	//������ ������
	m_MainMenu->SetInventory(m_Inventory);
	m_MainMenu->SetEquipment(m_Equipment);


	/*m_MainMenu->SetInventoryButtonClickCallback<UMainHUD>(this,
		&UMainHUD::InventoryButtonClick);*/


	//test
	m_Inventory->SetEquipClass(m_Equipment);
	m_Equipment->SetInventoryClass(m_Inventory);
	m_Equipment->InitEquipment();
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

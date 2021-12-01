// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"


void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	// 변수인지가 안되엉 있어야 여기서 가져다가 쓸 수 있다....
	// 이름으로 변수를 얻어올 수 있다.
	m_CharacterHUD = Cast<UCharacterHUD>(GetWidgetFromName(TEXT("UI_CharacterHUD")));
	m_MainMenu = Cast<UMainMenuWidget>(GetWidgetFromName(TEXT("UI_MainMenu")));
	m_Inventory = Cast<UInventory>(GetWidgetFromName(TEXT("UI_Inventory")));
	m_Equipment = Cast<UEquipment>(GetWidgetFromName(TEXT("UI_Equipment")));

	//뺄것들
	m_InventoryList = Cast<UInventoryList>(GetWidgetFromName(TEXT("UI_InventoryList")));
	m_InventoryTile = Cast<UInventoryTile>(GetWidgetFromName(TEXT("UI_InventoryTile")));
	m_Auction = Cast<UAuctionWidget>(GetWidgetFromName(TEXT("UI_AuctionWidget")));
	m_LandScapeSlider = Cast<USlider>(GetWidgetFromName(TEXT("LandScapeTiling")));

	//MainHUD 블루프린트에서 LandScapeTiling의 변수인지 체크 해제 한 후 디테일에서 이벤트 함수 이름들을 확인할 수 있다.
	//함수 이름 확인 후 다시 변수인지 체크 후 동적할당 하여 사용하면 됨
	//값 바뀔때마다 함수를 호출함
	m_LandScapeSlider->OnValueChanged.AddDynamic(this, &UMainHUD::LandScapeSliderValue);


	m_LandScapeCollection = LoadObject<UMaterialParameterCollection>(GetWorld(),
		TEXT("MaterialParameterCollection'/Game/LandScape/MCMainLandScapeData.MCMainLandScapeData'"));

	//인스턴스 만드는 방법 , 인스턴스 객체를 하나 만들어줌, 실제로 값을 컨트롤 하는 것은 인스턴트가 해줌
	m_LandScapeCollectionInst = GetWorld()->GetParameterCollectionInstance(m_LandScapeCollection);

	m_MainMenu->SetInventoryList(m_InventoryList);
	m_MainMenu->SetInventoryTile(m_InventoryTile);
	m_MainMenu->SetAuction(m_Auction);

	//실제로 쓴느것
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
	//제대로 되는건지 아닌지 불값으로 리턴해줌 
	//들어온 값(슬라이더로 조절한 값)을 랜드스캐이프 데이타의 타일링 data값에 set
	bool Find = m_LandScapeCollectionInst->SetScalarParameterValue(TEXT("Tiling"), Value);
}

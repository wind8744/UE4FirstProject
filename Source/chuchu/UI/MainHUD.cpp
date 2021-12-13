// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"


void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	// 변수인지가 안되엉 있어야 여기서 가져다가 쓸 수 있다....
	// 이름으로 변수를 얻어올 수 있다.
	m_CharacterHUD = Cast<UCharacterHUD>(GetWidgetFromName(TEXT("UI_CharacterHUD")));
	m_MainMenuSkill = Cast<UMainMenuSkillWidget>(GetWidgetFromName(TEXT("UI_MainMenuSkill")));
	m_MainMenu = Cast<UMainMenuWidget>(GetWidgetFromName(TEXT("UI_MainMenu")));
	m_Inventory = Cast<UInventory>(GetWidgetFromName(TEXT("UI_Inventory")));
	m_Equipment = Cast<UEquipment>(GetWidgetFromName(TEXT("UI_Equipment")));
	m_QuestWidget = Cast<UQuestWidget>(GetWidgetFromName(TEXT("UI_QUest")));
	
	//test
	m_Auction = Cast<UAuctionWidget>(GetWidgetFromName(TEXT("UI_AuctionWidget")));
	m_LandScapeSlider = Cast<USlider>(GetWidgetFromName(TEXT("LandScapeTiling")));

	//MainHUD 블루프린트에서 LandScapeTiling의 변수인지 체크 해제 한 후 디테일에서 이벤트 함수 이름들을 확인할 수 있다.
	//함수 이름 확인 후 다시 변수인지 체크 후 동적할당 하여 사용하면 됨
	//값 바뀔때마다 함수를 호출함
	m_LandScapeSlider->OnValueChanged.AddDynamic(this, &UMainHUD::LandScapeSliderValue);

	m_LandScapeCollection = LoadObject<UMaterialParameterCollection>(GetWorld(), TEXT("MaterialParameterCollection'/Game/LandScape/MCMainLandScapeData.MCMainLandScapeData'"));

	//인스턴스 만드는 방법 , 인스턴스 객체를 하나 만들어줌, 실제로 값을 컨트롤 하는 것은 인스턴트가 해줌
	m_LandScapeCollectionInst = GetWorld()->GetParameterCollectionInstance(m_LandScapeCollection);

	m_MainMenu->SetAuction(m_Auction);
	m_MainMenu->SetInventory(m_Inventory);
	m_MainMenu->SetEquipment(m_Equipment);
	m_MainMenu->SetQuestWidget(m_QuestWidget);

	/*m_MainMenu->SetInventoryButtonClickCallback<UMainHUD>(this,
		&UMainHUD::InventoryButtonClick);*/

	//장비창과 인벤토리에 서로의 주소값 세팅
	m_Inventory->SetEquipClass(m_Equipment);
	m_Equipment->SetInventoryClass(m_Inventory);
	m_Equipment->InitEquipment();

	//for drap &drop 상호작용을 위해
	Visibility = ESlateVisibility::Visible;
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
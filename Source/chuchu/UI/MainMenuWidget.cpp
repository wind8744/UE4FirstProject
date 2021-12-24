// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "CombineWidget.h"
#include "../chuchuGameModeBase.h"
#include "../UIItem/ItemData.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_UIBUtton = Cast<UButton>(GetWidgetFromName(TEXT("UIButton")));
	m_TxtBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("CoolTimeText1"))); //skill1
	m_TxtBlock2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("CoolTimeText2"))); //skill2
	m_TxtBlock3 = Cast<UTextBlock>(GetWidgetFromName(TEXT("CoolTimeText3"))); //skill3
	m_PotionTxt = Cast<UTextBlock>(GetWidgetFromName(TEXT("CoolTimeText4"))); //potion


	m_UIBUtton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnUIWidget); //퀘스트창
}

void UMainMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	//플레이어에 얻어와서 쿨타임 표시
	m_TxtBlock->SetText(FText::FromString(" ")); 
	m_TxtBlock2->SetText(FText::FromString(" "));
	m_TxtBlock3->SetText(FText::FromString(" "));
	
	//인벤토리에 얻어와서 포션 수 표시

	AchuchuGameModeBase* gameMode = Cast<AchuchuGameModeBase>(GetWorld()->GetAuthGameMode());
	if (gameMode)
	{
		bool finddata = false;
		const TArray<UItemData*> items =  gameMode->GetMainHUD()->GetInventory()->GetItems();
		for (int a = 0; a < items.Num(); ++a)
		{
			if (items[a]->GetNameText().Equals("Health", ESearchCase::CaseSensitive))
			{
				int32 ret = items[a]->GetIndex();
				m_PotionTxt->SetText(FText::FromString(FString::FromInt(ret+1)));
				finddata = true;
				break;
			}
		}
		if(!finddata)
		{
			m_PotionTxt->SetText(FText::FromString("0"));
		}

	}
	


}

// ============================
// 버튼이 눌릴때 호출되는 함수들
// ============================
void UMainMenuWidget::OnUIWidget()
{
	if (m_UIWidget->GetVisibility() == ESlateVisibility::Collapsed)
		m_UIWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	else
		m_UIWidget->SetVisibility(ESlateVisibility::Collapsed);
}
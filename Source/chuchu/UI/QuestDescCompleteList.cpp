// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestDescCompleteList.h"
#include "QuestDescCompleteListData.h"

void UQuestDescCompleteList::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_TypeText = Cast<UTextBlock>(GetWidgetFromName("TypeText"));
	m_DescText = Cast<UTextBlock>(GetWidgetFromName("TypeText2"));
	m_CountText = Cast<UTextBlock>(GetWidgetFromName("TypeText3"));



}

void UQuestDescCompleteList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

//데이터가 리스트로 들어왔을대 블프에 구현한 것처러 ㅁ여기로 들어온다.
void UQuestDescCompleteList::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UQuestDescCompleteListData* item = Cast<UQuestDescCompleteListData>(ListItemObject);

	//item->SetOwnerWidget(this);
	m_TypeText->SetText(FText::FromString(item->GetTypeText()));
	m_DescText->SetText(FText::FromString(item->GetDestText()));
	m_CountText->SetText(FText::FromString(item->GetCountText()));
	m_Index = item->GetIndex();
}

void UQuestDescCompleteList::NativeOnItemSelectionChanged(bool bIsSelected)
{

}
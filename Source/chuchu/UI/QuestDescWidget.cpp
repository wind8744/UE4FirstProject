// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestDescWidget.h"


void UQuestDescWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("QuestName")));
	m_DescText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_168")));
	m_CompleteList = Cast<UListView>(GetWidgetFromName(TEXT("CompleteList")));

	//m_CompleteList->OnItemClicked().AddUObject(this, &UQuestDescWidget::QuestcompleteClick);
}

void UQuestDescWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UQuestDescWidget::AddList(const FQuestDataInfo& _info, int32 _index)
{
	UQuestDescCompleteListData* QuestItemData = NewObject<UQuestDescCompleteListData>(this, UQuestDescCompleteListData::StaticClass());

	FString	TypeText;
	switch (_info.type)
	{
	case EQuestType::Collection:
	{
		TypeText = TEXT("수집");
	}
	break;
	case EQuestType::Hunt:
	{
		TypeText = TEXT("사냥");
	}
	break;
	case EQuestType::Communication:
	{
		TypeText = TEXT("대화");
	}
	break;
	}
	QuestItemData->SetIndex(_index);
	QuestItemData->SetTypeText(TypeText);
	QuestItemData->SetDestText(_info.DestName);
	QuestItemData->SetCountText(_info.Count, _info.MaxCount);
	m_CompleteList->AddItem(QuestItemData);
}
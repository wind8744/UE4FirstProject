// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestListWidget.h"
#include "QuestListData.h"

void UQuestListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameText")));
	m_Selection = Cast<UImage>(GetWidgetFromName(TEXT("Selection")));

	m_Select = false;
}

void UQuestListWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

//블루프린트의 event함수와 연결됨
void UQuestListWidget::SetData(UObject* Data)
{
	//퀘스트 데이터
	UQuestListData* Item = Cast<UQuestListData>(Data);

	Item->SetOwnerWidget(this);

	m_NameText->SetText(FText::FromString(Item->GetQuestName()));
}

void UQuestListWidget::Select(bool Enable)
{
	m_Select = Enable;

	if (Enable)
	{
		FLinearColor	color(1.f, 0.7f, 0.f, 0.5f);
		m_Selection->SetBrushTintColor(color);
	}
	else
	{
		FLinearColor	color(0.2f, 0.2f, 0.2f, 0.f);
		m_Selection->SetBrushTintColor(color);
	}
}

void UQuestListWidget::MouseOn(bool Enable)
{
	if (!m_Select)
	{
		if (Enable)
		{
			FLinearColor	color(0.2f, 0.2f, 0.2f, 0.5f);
			m_Selection->SetBrushTintColor(color);
		}
		else
		{
			FLinearColor	color(0.2f, 0.2f, 0.2f, 0.f);
			m_Selection->SetBrushTintColor(color);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "AuctionItemData.h"
#include "AuctionItem.h"

UAuctionItemData::UAuctionItemData()
{
	m_TreeLevel = 0;
}

void UAuctionItemData::CreateChildren(const FString& Name)
{
	//아이템이 또다른 자식을 가질 수 있음
	UAuctionItemData* Child = NewObject<UAuctionItemData>(this,
		UAuctionItemData::StaticClass());

	Child->m_TreeLevel = m_TreeLevel + 1; //그 아랫쪽 레벨이라고 표시 (자식이므로)
	//Child->m_NameText = FString::Printf(TEXT("Child%d-%d"), m_TreeLevel, i);
	Child->m_NameText = Name;
	Child->SetParent(this);

	m_ChildArray.Add(Child); //배열로 가지고 있을수 있게끔 작업
}

void UAuctionItemData::Selection()
{
	m_LinkItem->Selection();
}

void UAuctionItemData::UnSelection()
{
	m_LinkItem->UnSelection();
}

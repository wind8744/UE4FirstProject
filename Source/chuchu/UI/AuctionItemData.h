// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "AuctionItemData.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API UAuctionItemData : public UObject
{
	GENERATED_BODY()
public:
	UAuctionItemData();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString	m_NameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UObject*>	m_ChildArray;

	UAuctionItemData* m_Parent;

	int32	m_Index;
	int32	m_TreeLevel; //트리를 몇번 타고 내려오면서 만들어진 놈인지

	class UAuctionItem* m_LinkItem;

public:
	TArray<UObject*>& GetChildArray()
	{
		return m_ChildArray;
	}

public:
	void SetLinkItem(class UAuctionItem* Item)
	{
		m_LinkItem = Item;
	}

	void Selection();
	void UnSelection();

	void SetParent(UAuctionItemData* Parent) //부모를 가질 수 있게끔
	{
		m_Parent = Parent;
	}

	int32 GetTreeLevel()	const
	{
		return m_TreeLevel;
	}

	FString GetNameText()
	{
		return m_NameText;
	}

public:
	void SetNameText(const FString& NameText)
	{
		m_NameText = NameText;
	}

	void CreateChildren(const FString& Name);
};

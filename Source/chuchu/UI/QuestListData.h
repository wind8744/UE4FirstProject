// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "QuestListData.generated.h"

/**
 * 퀘스트 리스트에 들어가는 데이터
 * INventory의 아이템 데이터 같은
 * 정보만 가지고 있음
 */
UCLASS(BlueprintType)
class CHUCHU_API UQuestListData : public UObject
{
	GENERATED_BODY()
	
public:
	UQuestListData();

private:
	FString m_Name;
	int32 m_Index;
	class UQuestListWidget* m_OwnerWidget;

public:
	class UQuestListWidget* GetOwnerWidget() { return m_OwnerWidget; }
	void SetOwnerWidget(class UQuestListWidget* _widget) { m_OwnerWidget = _widget; }

	const FString& GetQuestName() const { return m_Name; }
	void SetQuestName(FString _name) { m_Name = _name; }

	int32 GetIndex() { return m_Index; }
	void SetIndex(int32 _index) { m_Index = _index; }

};

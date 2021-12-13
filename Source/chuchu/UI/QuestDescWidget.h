// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "QuestDescCompleteListData.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "Blueprint/UserWidget.h"
#include "QuestDescWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API UQuestDescWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_NameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_DescText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UListView* m_CompleteList;

public:
	void SetQuestName(const FString& _name) { m_NameText->SetText(FText::FromString(_name)); }
	void SetQuestDesc(const FString& _name) { m_DescText->SetText(FText::FromString(_name)); }
	void ClearList() { m_CompleteList->ClearListItems(); }	// list view 클리어
	void AddList(const FQuestDataInfo& _info, int32 _index);

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	//UFUNCTION()
	//	void QuestCompleteClick(UObject* Data);
};
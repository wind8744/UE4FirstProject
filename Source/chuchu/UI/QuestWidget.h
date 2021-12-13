// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "QuestDescWidget.h"
#include "Components/ListView.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "QuestWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UListView* m_QuestList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UQuestDescWidget* m_QuestDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_CloseButton;

	FQuestData* m_CurRenderQuest;

	TArray<FQuestData> m_QuestArray;
	TArray<FQuestData*> m_QuestListArray[(uint8)EQuestType::Max];

	class UQuestListData* m_PrevSelect;

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	//widget Drag&drop
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation); //드래그 감지

public:
	UFUNCTION()
		void CloseButtonClick();
	void CloseInvenUI(); //장비창에서 창 닫을때

	UFUNCTION()
		void QuestClick(UObject* Data);

	UFUNCTION()
		void QuestSelect(UObject* Data);

	UFUNCTION()
		void QuestMouseOn(UObject* Data, bool IsHovered);
public:
	void QuestCheck(EQuestType _type, const FString& Name);
};
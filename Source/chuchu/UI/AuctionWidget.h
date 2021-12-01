// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/TreeView.h"
#include "Blueprint/UserWidget.h"
#include "AuctionWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API UAuctionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTreeView* m_Menu;

	class UAuctionItemData* m_SelectItem;

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

public:
	UFUNCTION()
		void ItemDoubleClick(UObject* Data);

	UFUNCTION()
		void ItemHOvered(UObject* Data, bool Hovered);

	/*
	*auctionWidget ��������  Tree�� �����Ͽ���
	*�̺�Ʈ ���� OnGetItemChildren �Լ��� �ִµ�
	*C++ �ڵ忡�� �θ� �� �ִµ� �θ��� Ʈ�� ���·� �ؿ� ���� �ڽĵ�
	*�� �����´�.
	*/
	UFUNCTION()
		void GetItemChildren(UObject* Data, TArray<UObject*>& ItemArray);
};

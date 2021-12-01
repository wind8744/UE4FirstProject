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
	*auctionWidget 블프에서  Tree의 디테일에서
	*이벤트 보면 OnGetItemChildren 함수가 있는데
	*C++ 코드에서 부를 수 있는데 부르면 트리 형태로 밑에 딸린 자식들
	*다 가져온다.
	*/
	UFUNCTION()
		void GetItemChildren(UObject* Data, TArray<UObject*>& ItemArray);
};

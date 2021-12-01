// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/ListView.h"
#include "InventoryItemList.h"
#include "InventoryItemDataList.h"
#include "Blueprint/UserWidget.h"
#include "InventoryList.generated.h"



UCLASS()
class CHUCHU_API UInventoryList : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UListView* m_InventoryList;

public:

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UFUNCTION()
		void ItemClick(UObject* Data);
};

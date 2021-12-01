// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfo.h"

void UItemInfo:: NativeConstruct()
{
	Super::NativeConstruct();

	m_ItemTEx = Cast<UImage>(GetWidgetFromName(TEXT("Icon")));
	m_ItemName= Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	m_ItemInfo =Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemInfo")));

}

void UItemInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

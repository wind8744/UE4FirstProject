// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "ItemInfo.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API UItemInfo : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UImage* m_ItemTEx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_ItemInfo;

public:
	void SetItemName(FString _Name) { m_ItemName->SetText(FText::FromString(_Name)); }
	void SetItemInfo(FString _Info) { m_ItemInfo->SetText(FText::FromString(_Info)); }
	void SetItemTex(UTexture2D* _TEx) { m_ItemTEx->SetBrushFromTexture(_TEx); }

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "ItemDescWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API UItemDescWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UImage* m_IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_NameText;

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	void SetNameText(const FString& NameText)
	{
		m_NameText->SetText(FText::FromString(NameText));
	}

	void SetIconTexture(UTexture2D* IconTexture)
	{
		m_IconImage->SetBrushFromTexture(IconTexture);
	}
};

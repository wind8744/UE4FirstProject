// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/ProgressBar.h"
#include "Slate/SlateBrushAsset.h"
#include "Blueprint/UserWidget.h"
#include "CoolTimeBar.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API UCoolTimeBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UProgressBar* m_TimeBar;

public:
	void SetCoolTimePercent(float Percent)
	{
		m_TimeBar->SetPercent(Percent);
	}

protected:
	virtual void NativeOnInitialized();
	virtual void NativePreConstruct();
	virtual void NativeConstruct();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../GameInfo.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "StartWidget.generated.h"
/**
 * 
 */
UCLASS()
class CHUCHU_API UStartWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton* m_StartButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton* m_QuitButton;

	//start animation
	//UPROPERTY(meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"))
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UWidgetAnimation* m_StartButtonMouseOn; //이름 똑같아야함

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

protected:
	void BindingAnimations();

public:
	UFUNCTION()
	void StartButton();

	UFUNCTION()
	void QuitButton();

	UFUNCTION()
	void StartButtonHOvered();

	UFUNCTION()
	void QuitButtonHOvered();
};

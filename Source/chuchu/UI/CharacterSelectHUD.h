// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectHUD.generated.h"
/**
 * 
 */
UCLASS()
class CHUCHU_API UCharacterSelectHUD : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_StartButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_BackButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UCharacterSelectState* m_KnightState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UCharacterSelectState* m_ArcherState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UCharacterNameInputWidget* m_NameInput;



protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UFUNCTION()
		void StartButtonClick();

	UFUNCTION()
		void BackButtonClick();

public:
	//ui_characterselecthud에서 startbutton 의 behavior에서 Is Enabled에 체크를 해제하면 버튼이 비활성화 되고
	//캐릭터를 선택하여 골랐을 때 버튼이 활성화 될 수 있도록 할 것
	void EnableStartButton(bool Enable)
	{
		m_StartButton->SetIsEnabled(Enable);
	}

	void SetKnightStateVisibility(ESlateVisibility Visible);
	void SetArcherStateVisibility(ESlateVisibility Visible);
	void SetInputNameVisibility(ESlateVisibility Visible);
	void SetKnightInfo(const FPlayerInfo& Info);
	void SetArcherInfo(const FPlayerInfo& Info);
};

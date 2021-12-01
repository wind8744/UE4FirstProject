// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"

//이미지를 가져다 쓰고 싶을때 
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include "Blueprint/UserWidget.h"
#include "CharacterHUD.generated.h"

DECLARE_DELEGATE(FNameDelegate);

UCLASS()

class CHUCHU_API UCharacterHUD : public UUserWidget
{
	GENERATED_BODY()
protected:
	//이미지들을 가져다 쓸것이므로
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UImage* m_CharacterPic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_NameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UProgressBar* m_HPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UProgressBar* m_MPBar;

	FNameDelegate	m_NameDelegate;
	FString			m_InputName;

public:
	void SetInputName(const FString& Name)
	{
		m_InputName = Name;
	}

	void SetHPPercent(float Percent)
	{
		m_HPBar->SetPercent(Percent);
	}

	void SetMPPercent(float Percent)
	{
		m_MPBar->SetPercent(Percent);
	}

	void SetName(const FString& Name)
	{
		m_NameText->SetText(FText::FromString(Name));
	}

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	template <typename T>
	void SetDelegate(T* Obj, void(T::* Func)())
	{
		m_NameDelegate.BindUObject(Obj, Func);
	}
};

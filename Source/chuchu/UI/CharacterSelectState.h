// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectState.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API UCharacterSelectState : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UImage* m_CharacterImage; //캐릭터 정보들

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_NameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_JobText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_AttackText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_ArmorText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_HPText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_MPText;

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	void SetImageMaterial(UMaterialInterface* Material);
	void SetNameText(const FString& Name) 
	{
		m_NameText->SetText(FText::FromString(Name)); //텍스트 상자에 TEXT 표시, 정보 설정 ,  SET TEXT 
	}

	void SetJobText(EPlayerJob Job)
	{
		FString	JobName;
		switch (Job)
		{
		case EPlayerJob::Knight:
			JobName = TEXT("직업 : 기사");
			break;
		case EPlayerJob::Archer:
			JobName = TEXT("직업 : 궁수");
			break;
		case EPlayerJob::Magicion:
			JobName = TEXT("직업 : 마법사");
			break;
		}

		m_JobText->SetText(FText::FromString(JobName));
	}

	void SetAttackText(int32 Attack)
	{
		m_AttackText->SetText(FText::FromString(FString::Printf(TEXT("공격력 : %d"), Attack)));
	}

	void SetArmorText(int32 Armor)
	{
		m_ArmorText->SetText(FText::FromString(FString::Printf(TEXT("방어력 : %d"), Armor)));
	}

	void SetHPText(int32 HP, int32 HPMax)
	{
		m_HPText->SetText(FText::FromString(FString::Printf(TEXT("체력 : %d / %d"), HP, HPMax)));
	}

	void SetMPText(int32 MP, int32 MPMax)
	{
		m_MPText->SetText(FText::FromString(FString::Printf(TEXT("체력 : %d / %d"), MP, MPMax)));
	}
};

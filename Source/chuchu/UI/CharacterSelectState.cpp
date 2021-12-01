// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterSelectState.h"

void UCharacterSelectState::NativeConstruct()
{
	Super::NativeConstruct();
	//CharacterImage �������� CharacterImage�̸��� ������ �������°��̹Ƿ� �� ������ �̸��� �־��־�� �Ѵ�
	m_CharacterImage = Cast<UImage>(GetWidgetFromName(TEXT("CharacterImage"))); 
	m_NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name")));
	m_JobText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Job")));
	m_AttackText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Attack")));
	m_ArmorText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Armor")));
	m_HPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HP")));
	m_MPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MP")));
}

void UCharacterSelectState::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCharacterSelectState::SetImageMaterial(UMaterialInterface* Material)
{	
	//�̹��� ����
	m_CharacterImage->SetBrushFromMaterial(Material);
}

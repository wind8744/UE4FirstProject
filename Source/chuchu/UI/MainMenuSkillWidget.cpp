// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuSkillWidget.h"

void UMainMenuSkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_DrinkButton = Cast<UButton>(GetWidgetFromName(TEXT("drink")));
	m_Skill1Button = Cast<UButton>(GetWidgetFromName(TEXT("Skill1")));
	m_Skill2Button = Cast<UButton>(GetWidgetFromName(TEXT("Skill2")));
	m_Skill3Button = Cast<UButton>(GetWidgetFromName(TEXT("Skill3")));

}

void UMainMenuSkillWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//플레이어의 스킬 쿨타임 가져와서 이미지 변경

}
// Fill out your copyright notice in the Description page of Project Settings.


#include "CoolTimeBar.h"
#include "../chuchuGameInstance.h"
#include "../Player/PlayerCharacter.h"

void UCoolTimeBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_TimeBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("UI_ProgressBar")));
}
void UCoolTimeBar::NativePreConstruct()
{
	Super::NativePreConstruct();
}
void UCoolTimeBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCoolTimeBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

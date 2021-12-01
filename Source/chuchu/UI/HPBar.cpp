// Fill out your copyright notice in the Description page of Project Settings.


#include "HPBar.h"
#include "../chuchuGameInstance.h"
#include "../Player/PlayerCharacter.h"

void UHPBar::NativeConstruct()
{
	Super::NativeConstruct();

	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Bar")));
	m_NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name1")));

	if (m_NameDelegate.IsBound()) //바인딩이 되었는지 안되었는지 화인 
	{
		m_NameDelegate.Execute();
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		//내거
		//인스턴스에 이름 저장해놓은것 불러와서 ui에 set
		//UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());
		//const FString playername = GameInst->GetPlayerInfoName();
		//SetName(playername);
	}
}

void UHPBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "HPBar.h"
#include "../chuchuGameInstance.h"
#include "../Player/PlayerCharacter.h"

void UHPBar::NativeConstruct()
{
	Super::NativeConstruct();

	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Bar")));
	m_NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name1")));

	if (m_NameDelegate.IsBound()) //���ε��� �Ǿ����� �ȵǾ����� ȭ�� 
	{
		m_NameDelegate.Execute();
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		//����
		//�ν��Ͻ��� �̸� �����س����� �ҷ��ͼ� ui�� set
		//UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());
		//const FString playername = GameInst->GetPlayerInfoName();
		//SetName(playername);
	}
}

void UHPBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}



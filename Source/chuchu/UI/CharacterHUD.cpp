// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterHUD.h"
#include "../Player/PlayerCharacter.h"
#include "../chuchuGameInstance.h"

void UCharacterHUD::NativeConstruct()
{
	Super::NativeConstruct();

	//m_CharacterPic = Cast<UImage>(GetWidgetFromName(TEXT("CharacterPic")));
	m_NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("charname")));
	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	//m_MPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("MPBar")));

	/*if (m_NameDelegate.IsBound())
	{
		m_NameDelegate.Execute();
	}*/

	/*if (GetWorld())
	{
		PrintViewport(10.f, FColor::Red, TEXT("World"));

		if (GetWorld()->GetFirstPlayerController())
		{
			PrintViewport(10.f, FColor::Red, TEXT("PlayerController"));
		}
	}*/


	APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		//������
		//SetName(Player->GetPlayerInfo().Name);

		//����
		//�ν��Ͻ��� �̸� �����س����� �ҷ��ͼ� ui�� set
		UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());
		FString playername = "CharacterName";//GameInst->GetPlayerInfoName();		

		const FPlayerTableInfo* PlayerInfo = GameInst->FindPlayerInfo("Knight");
		playername = PlayerInfo->Name;
		SetName(playername);
	}
}

void UCharacterHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}



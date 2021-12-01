// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameInfo.h"
#include "UI/MainHUD.h"
#include "GameFramework/GameModeBase.h"
#include "AssetManager/AssetPathMain.h"
#include "chuchuGameModeBase.generated.h"

UCLASS()
class CHUCHU_API AchuchuGameModeBase : public AGameModeBase 
{
	GENERATED_BODY()

public:
	AchuchuGameModeBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMainHUD>	m_MainHUDClass; //���� ui class�� ���´�

	TArray<TSubclassOf<APawn>>	m_PlayerClassArray;

	UMainHUD* m_MainHUD; //UI ���� hud�� ���������� �� �ֵ��� ���� ����
	UAssetPathMain* m_MainAssetPath;

public:
	UMainHUD* GetMainHUD()
	{
		return m_MainHUD;
	}


public:
	//���� �ʱ�ȭ�ɶ� �Ҹ��� �Լ�, ĳ���� ����â ����� ���� ��������
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void BeginPlay() override;
};

/**
 * �⺻ ���� ���� ���
 */
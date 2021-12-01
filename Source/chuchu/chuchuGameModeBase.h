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
	TSubclassOf<UMainHUD>	m_MainHUDClass; //메인 ui class를 얻어온다

	TArray<TSubclassOf<APawn>>	m_PlayerClassArray;

	UMainHUD* m_MainHUD; //UI 메인 hud를 가지고있을 수 있도록 변수 선언
	UAssetPathMain* m_MainAssetPath;

public:
	UMainHUD* GetMainHUD()
	{
		return m_MainHUD;
	}


public:
	//게임 초기화될때 불리는 함수, 캐릭터 선택창 만들기 위해 만들어놓음
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void BeginPlay() override;
};

/**
 * 기본 게임 모드로 사용
 */
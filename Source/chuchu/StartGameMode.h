// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "UI/StartWidget.h"
#include "GameFramework/GameModeBase.h"
#include "StartGameMode.generated.h"
/**
 * 
 */
UCLASS()
class CHUCHU_API AStartGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AStartGameMode();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UStartWidget>	m_StartHUDClass; //À§Á¬ Å¬·¡½º 

	UStartWidget* m_StartHUD;

public:
	UStartWidget* GetStartHUD()
	{
		return m_StartHUD;
	}

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void BeginPlay() override;

};

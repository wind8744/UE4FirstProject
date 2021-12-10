// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	AMainPlayerController();

private:
	TArray<class AEnvironmentNormalActor*>	m_CollisionEnvironmentActor;

	bool m_MouseClicked;
	bool m_MouseOn;
	bool m_IsUIMode;

public:
	virtual void PlayerTick(float DeltaTime);
	virtual void SetupInputComponent();

private:
	void MouseClick();
	void MouseReleased();
	void PickingItem();
	void UIKey();

public:
	bool IsMouseOn() { return m_MouseOn; }
};

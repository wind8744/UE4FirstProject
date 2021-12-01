// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BoxTrigger.h"
#include "MeteorTrigger.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API AMeteorTrigger : public ABoxTrigger
{
	GENERATED_BODY()

public:
	AMeteorTrigger();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		void TriggerBegin();

	UFUNCTION()
		void TriggerEnd();
};

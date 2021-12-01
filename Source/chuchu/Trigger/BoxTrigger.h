// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Trigger.h"
#include "BoxTrigger.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API ABoxTrigger : public ATrigger
{
	GENERATED_BODY()
public:
	ABoxTrigger();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};

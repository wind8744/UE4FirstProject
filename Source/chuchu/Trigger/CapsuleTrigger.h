// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Trigger.h"
#include "CapsuleTrigger.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API ACapsuleTrigger : public ATrigger
{
	GENERATED_BODY()
	
public:
	ACapsuleTrigger();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};

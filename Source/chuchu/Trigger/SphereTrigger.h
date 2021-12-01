// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Trigger.h"
#include "SphereTrigger.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API ASphereTrigger : public ATrigger
{
	GENERATED_BODY()
public:
	ASphereTrigger();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};

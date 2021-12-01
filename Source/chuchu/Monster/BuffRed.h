// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "../GameInfo.h"
#include "Buff.h"
#include "BuffRed.generated.h"

/**
 *
 */
UCLASS()
class CHUCHU_API ABuffRed : public ABuff
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABuffRed();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void NormalAttack();
	virtual void SpawnEnd();

};

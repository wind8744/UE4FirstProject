// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Monster.h"
#include "BossRampage.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API ABossRampage : public AMonster
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossRampage();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
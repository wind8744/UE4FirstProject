// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "Minion.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API AMinion : public AMonster
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMinion();

protected:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

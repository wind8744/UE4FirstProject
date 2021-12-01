// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Minion.h"
#include "MinionGunner.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API AMinionGunner : public AMinion
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMinionGunner();

protected:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void NormalAttack();
};

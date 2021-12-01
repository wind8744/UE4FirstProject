// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "EffectDecal.h"
#include "GreyStoneSkill1Decal.generated.h"

/**
 *
 */
UCLASS()
class CHUCHU_API AGreyStoneSkill1Decal : public AEffectDecal
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGreyStoneSkill1Decal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

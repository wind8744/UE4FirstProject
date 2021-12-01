// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "DestructibleProjectile.h"
#include "GreyStoneSkill1.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API AGreyStoneSkill1 : public ADestructibleProjectile
{
	GENERATED_BODY()	

public:
	// Sets default values for this actor's properties
	AGreyStoneSkill1();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* m_Particle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* m_Trail;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void StopEvent(const FHitResult& result);
};

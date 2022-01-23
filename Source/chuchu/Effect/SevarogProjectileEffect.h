// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "ParticleProjectile.h"
#include "SevarogProjectileEffect.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API ASevarogProjectileEffect : public AParticleProjectile
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ASevarogProjectileEffect();

private:
	int32	m_Attack;
	class AMonster* m_Owner;
	float			m_fDestroyTime;

public:
	void SetAttack(int32 Attack)
	{
		m_Attack = Attack;
	}

	void SetOwner(class AMonster* Monster)
	{
		m_Owner = Monster;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void StopEvent(const FHitResult& result);
};

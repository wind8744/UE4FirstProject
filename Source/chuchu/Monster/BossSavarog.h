// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Monster.h"
#include "Components/CapsuleComponent.h"
#include "BossSavarog.generated.h"

/**
 * 
 */
enum class EBossState : uint8
{
	IDLE,
	TRACE,
	ATTACK,
	HIT_RCV,
	RUNAWAY,
	DEATH
};

UCLASS()
class CHUCHU_API ABossSavarog : public AMonster
{
	GENERATED_BODY()

public:
	ABossSavarog();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* m_CollisionCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*>	m_AttackMontageArray;

	EBossState					m_BossState;
	UMaterialInterface*			m_RedMat;
	TArray<UMaterialInterface*> m_CurMatArray;
	FTimerHandle				m_OriMatTimer;

	bool						m_bStart;
	bool						m_bAttackEnd;
	bool						m_bInDistance;
	bool						m_bAnim;
	float						m_fAtime;
	float						m_fTarDis;
	int32						m_iSkillNum;

private:
	void ChangeState();
	bool FindTarget();
	void Trace(float _dt);
	void Attack(float _dt);
	void HitRcv(float _dt);
	void RunAway(float _dt);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NormalAttack();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	virtual void Death();

};

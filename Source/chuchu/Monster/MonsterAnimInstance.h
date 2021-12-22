// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EMonsterAnimType : uint8
{
	Spawn,
	Idle,
	Walk,
	Run,
	Attack,
	Death,
	Skill,
	Skill2,
	Skill3
};

UENUM(BlueprintType)
enum class EMosnterAttackType : uint8
{
	Attack1,
	Attack2,
	Attack3,
	Attack4
};

UCLASS()
class CHUCHU_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMonsterAnimInstance();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool	m_OnGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EMonsterAnimType	m_AnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EMosnterAttackType	m_AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	m_Hit;

public:
	EMonsterAnimType GetAnimType()	const
	{
		return m_AnimType;
	}

	void ChangeAnimType(EMonsterAnimType Type)
	{
		m_AnimType = Type;
	}

	void SetHit()
	{
		m_Hit = 1.f;
	}

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	UFUNCTION()
		void AnimNotify_Attack();

	UFUNCTION()
		void AnimNotify_AttackEnd();

	UFUNCTION()
		void AnimNotify_DeathEnd();

	UFUNCTION()
		void AnimNotify_HitEnd();

	UFUNCTION()
		void AnimNotify_SpawnEnd();
};

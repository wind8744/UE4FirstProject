// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

UENUM(BlueprintType) //블루프린트로 사용할 수 있는것을 알려주어야 함
enum class EPlayerAnimType : uint8
{
	Ground,
	Jump,
	Fall,
	Death,
	Avoid,
	Dash,
};

UCLASS()
class CHUCHU_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerAnim();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))  //멤버변수에 붙일수 있음
		bool m_Attack; //현재 공격상태 인지 아닌지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool	m_OnGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))  //멤버변수에 붙일수 있음
		EPlayerAnimType m_AnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	m_FallRecoveryAdditive;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool m_DidEquipWeapon; //장착

	float m_fRun;

public:
	float GetDirection()
	{
		return m_Direction;
	}
	void SetDirection(float Direction) { m_Direction = Direction; }
	void SetAttackEnable(bool Attack) { m_Attack = Attack; } //공격상태로 바꾸어줌
	void SetSpeed(float Speed) { m_fRun = Speed; }

	bool IsAttack() const
	{
		return m_Attack;
	}

	void ChangeAnimType(EPlayerAnimType Type)
	{
		m_AnimType = Type;
	}

	EPlayerAnimType GetAnimType()
	{
		return m_AnimType;
	}

	void InitWeaponAnimPose(); //플레이어 무기 탈착시 호출

	//void SetDidEquipWeapon(bool Value) { m_DidEquipWeapon = Value; }

public:
	virtual void NativeInitializeAnimation();
	// Native update override point. It is usually a good idea to simply gather data in this step and 
	// for the bulk of the work to be done in NativeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	// 노티파이가 호출될 때 함수를 만들기 위해서는 규칙이 있다
	// AnimNotify_를 붙여햐 한다.
	// 애니메이션이 실행될 때 노티파이를 붙힌 곳에 도착했을대 실행되는 함수들

	UFUNCTION() //멤버함수에 붙일 수 있다.
	void AnimNotify_AttackEnd();

	UFUNCTION()
		void AnimNotify_NormalAttack();

	UFUNCTION()
		void AnimNotify_AttackCombo();

	UFUNCTION()
		void AnimNotify_JumpEnd();

	UFUNCTION()
		void AnimNotify_FallStart();

	UFUNCTION()
		void AnimNotify_FallEnd();

	UFUNCTION()
		void AnimNotify_FallRecoveryEnd();

	UFUNCTION()
		void AnimNotify_SkillFire();

	UFUNCTION()
		void AnimNotify_LeftStep();

	UFUNCTION()
		void AnimNotify_RightStep();

	UFUNCTION()
		void AnimNotify_OnMagicCircle();

	UFUNCTION()
		void AnimNotify_Skill2Fire();

	UFUNCTION()
		void AnimNotify_GhostTrailEnd();

	UFUNCTION()
		void AnimNotify_GhostTrailStart();

	UFUNCTION()
		void AnimNotify_Skill2();

	UFUNCTION()
		void AnimNotify_EquipEnd();// { m_DidEquipWeapon = true; m_AnimType = EPlayerAnimType::Ground; }
};

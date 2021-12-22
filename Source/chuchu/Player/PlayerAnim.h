// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

UCLASS()
class CHUCHU_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerAnim();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	 m_Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	 m_Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))  //��������� ���ϼ� ����
		bool	 m_Attack; //���� ���ݻ��� ���� �ƴ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool	m_OnGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))  //��������� ���ϼ� ����
		EPlayerAnimType		m_AnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	m_FallRecoveryAdditive;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool m_DidEquipWeapon; //����

	float m_fRun;

public:
	float GetDirection()
	{
		return m_Direction;
	}
	void SetDirection(float Direction) { m_Direction = Direction; }
	void SetAttackEnable(bool Attack) { m_Attack = Attack; } //���ݻ��·� �ٲپ���
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

	void InitWeaponAnimPose(); //�÷��̾� ���� Ż���� ȣ��

public:
	virtual void NativeInitializeAnimation();
	// Native update override point. It is usually a good idea to simply gather data in this step and 
	// for the bulk of the work to be done in NativeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	// ��Ƽ���̰� ȣ��� �� �Լ��� ����� ���ؼ��� ��Ģ�� �ִ�
	// AnimNotify_�� �ٿ��� �Ѵ�.
	// �ִϸ��̼��� ����� �� ��Ƽ���̸� ���� ���� ���������� ����Ǵ� �Լ���

	UFUNCTION() //����Լ��� ���� �� �ִ�.
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

	UFUNCTION()
		void AnimNotify_AvoidEnd();

	// ghost lady ~ skill start , end
	
	UFUNCTION()
		void AnimNotify_SkillStart();     // ��� ��ų ���� ~
									      
	UFUNCTION()						      
		void AnimNotify_SkillEnd();       // ��� ��ų ���� ~

	UFUNCTION()
		void AnimNotify_Skill2Shake();    // skill2 ī�޶� ����ũ ����Ʈ

	UFUNCTION()						      
		void AnimNotify_LoopStart();      // skill3 Ű�ٿ� ��Ÿ�� ����

	UFUNCTION()
		void AnimNotify_TimeDelayStart(); // skill4 ī�޶� ����ũ ����Ʈ

	UFUNCTION()
		void AnimNotify_TimeDelayEnd();   // skill4 ī�޶� ����ũ ����Ʈ

};

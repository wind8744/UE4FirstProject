// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerCharacter.h"

UPlayerAnim::UPlayerAnim()
{
	m_Direction = 0.f;
	m_Speed = 0.f;
	m_OnGround = true;
	m_AnimType = EPlayerAnimType::Ground;
	m_FallRecoveryAdditive = 0.f;
	m_fRun = 0.f;
	m_DidEquipWeapon = false; //���� ���� �� , �� �ִ� ���� //**
	//m_DidEquipWeapon = true;
}

void UPlayerAnim::NativeInitializeAnimation() //�ʱ�ȭ �� �� 
{
	Super::NativeInitializeAnimation();
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds) //�����Ӹ��� ȣ��Ǵ� �Լ�
{	
	Super::NativeUpdateAnimation(DeltaSeconds);

	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
	{
		UCharacterMovementComponent* Movement = Player->GetCharacterMovement();
		if (Movement)
		{
			//�ӵ� ����
			m_Speed = Movement->Velocity.Size();
				
			// ���� ��� �ִ��� ���Ѵ�.
			bool OnGround = Movement->IsMovingOnGround();

			//if(!m_OnGround && OnGround && Player->IsMoveKey())
			//	m_AnimType = EPlayerAnimType::Ground;

			m_OnGround = OnGround;

			// ���ĸ� ������� �������� ������� ���� �ɸ�
			if (m_OnGround && m_AnimType == EPlayerAnimType::Fall)
			{
				m_AnimType = EPlayerAnimType::Ground;
			}

			if (!m_OnGround && m_AnimType != EPlayerAnimType::Jump && m_AnimType != EPlayerAnimType::Avoid && m_AnimType != EPlayerAnimType::Skill)
			{
				m_AnimType = EPlayerAnimType::Fall;
				m_FallRecoveryAdditive = 0.f;
			}
			
			//�ȴ� �ӵ� ����
			Movement->MaxWalkSpeed = 600.f + (600.f * m_fRun); //�ȱ�ӵ� + �� �� �ӵ�
		}
	}
}

void UPlayerAnim::AnimNotify_AttackEnd()
{
	m_Attack = false;
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->AttackEnd();
}

void UPlayerAnim::AnimNotify_NormalAttack()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->NormalAttack();
}

void UPlayerAnim::AnimNotify_AttackCombo()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->EnableAttack(true);
}

// skill 1
void UPlayerAnim::AnimNotify_SkillFire()
{
	m_Attack = false;
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
	{
		Player->UseSkill(0);
		Player->AttackEnd();
	}
}

// skill 2
void UPlayerAnim::AnimNotify_Skill2()
{
	m_Attack = false;
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
	{
		Player->UseSkill(1);
		Player->AttackEnd();
	}
}

void UPlayerAnim::AnimNotify_JumpEnd()
{
	m_AnimType = EPlayerAnimType::Fall;
}

void UPlayerAnim::AnimNotify_FallEnd()
{
	m_AnimType = EPlayerAnimType::Ground;

	m_FallRecoveryAdditive = 1.f;

	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->PlayFallRecovery();
}

void UPlayerAnim::AnimNotify_FallRecoveryEnd()
{
	m_FallRecoveryAdditive = 0.f;
}

// ���� ���������Ƿ� �ٴ��� ������ Ȯ���ؾ� �Ѵ�.
void UPlayerAnim::AnimNotify_FallStart()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->FootStep(false);
}

void UPlayerAnim::AnimNotify_LeftStep()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->FootStep(true);
}

void UPlayerAnim::AnimNotify_RightStep()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->FootStep(false);
}

void UPlayerAnim::AnimNotify_OnMagicCircle()
{
	m_Attack = false;
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
	{
		Player->UseSkill(1);
		Player->AttackEnd();
	}
}

void UPlayerAnim::AnimNotify_Skill2Fire()
{
	m_Attack = false;
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
	{
		Player->UseSkillFire(1);
		Player->AttackEnd();
	}
}

void UPlayerAnim::AnimNotify_GhostTrailEnd()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->GhostTrailEnd();
}

void UPlayerAnim::AnimNotify_GhostTrailStart()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->OnGhostTrail();
}

void UPlayerAnim:: AnimNotify_EquipEnd()
{ 
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (Player)
		Player->ChangeWeaponSocket();

	m_DidEquipWeapon = true; 
	m_AnimType = EPlayerAnimType::Ground; 
}

void UPlayerAnim::AnimNotify_AvoidEnd()
{
	m_AnimType = EPlayerAnimType::Ground;
}
	
void UPlayerAnim::InitWeaponAnimPose()
{
	m_DidEquipWeapon = false;
	m_AnimType = EPlayerAnimType::Ground;
}


void UPlayerAnim::AnimNotify_SkillStart()
{
	m_Attack = false;
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
	{
		Player->UseSkill();
		Player->AttackEnd();
	}
}

void UPlayerAnim::AnimNotify_SkillEnd()
{
	m_AnimType = EPlayerAnimType::Ground;
}

void UPlayerAnim::AnimNotify_LoopStart()
{
	//m_Attack = true;

	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	Player->Skill3Loop();
}

void UPlayerAnim::AnimNotify_Skill2Shake()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	Player->ShakeCamera();
}

void UPlayerAnim:: AnimNotify_TimeDelayStart()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	Player->TimeDilation(0.2f);
	// Ghost Trail On
	// ++ ī�޶� Ȯ�� �� �̵�? �׷��͵� �߰��ϸ� ������ ����
}

void UPlayerAnim::AnimNotify_TimeDelayEnd()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	Player->TimeDilation(1.f);
}

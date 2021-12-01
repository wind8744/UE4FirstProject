// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"
#include "Monster.h"

UMonsterAnimInstance::UMonsterAnimInstance()
{
	m_OnGround = true;

	m_AnimType = EMonsterAnimType::Idle;
	
	m_Hit = 0.f;
}

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (Monster)
	{
		UCharacterMovementComponent* Movement = Monster->GetCharacterMovement();

		if (Movement)
		{
			// 땅을 밟고 있는지 구한다.
			bool OnGround = Movement->IsMovingOnGround();

			m_OnGround = OnGround;
		}
	}
}

void UMonsterAnimInstance::AnimNotify_Attack()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (Monster)
		Monster->NormalAttack(); //몬스터 노말어택 호출
}

void UMonsterAnimInstance::AnimNotify_AttackEnd()
{
	//PrintViewport(1.f, FColor::Red, TEXT("AttackEnd"));
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (Monster)
		Monster->SetAttackEnd(true);
}

void UMonsterAnimInstance::AnimNotify_DeathEnd()
{
	//노티파이가 불릴때
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (Monster)
		Monster->Death(); //몬스터에게 죽음을 알려줌
}

void UMonsterAnimInstance::AnimNotify_HitEnd()
{
	m_Hit = 0.f;
}

void UMonsterAnimInstance::AnimNotify_SpawnEnd()
{
	m_AnimType = EMonsterAnimType::Idle;
	//AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());
	//if (Monster)
	//	Monster->SpawnEnd();
}
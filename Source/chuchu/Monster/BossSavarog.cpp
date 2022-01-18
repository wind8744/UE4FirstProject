// Fill out your copyright notice in the Description page of Project Settings.


#include "BossSavarog.h"
#include "../Player/PlayerCharacter.h"
#include "../Effect/NormalEffect.h"

ABossSavarog::ABossSavarog()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Skins/Tier_1/Sevarog_Red/Meshes/SevarogBloodred.SevarogBloodred'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("AnimBlueprint'/Game/Monster/BPBossSevarogAnim.BPBossSevarogAnim_C'"));
	if (AnimAsset.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
	}

	//몽타주 (Skill)
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackAsset(TEXT("AnimMontage'/Game/Monster/AMSevarAttack.AMSevarAttack'"));
	if (AttackAsset.Succeeded())
		m_AttackMontageArray.Add(AttackAsset.Object);

	m_MonsterInfoName = TEXT("BossSavarog");
	m_DropItemArray.Add(TEXT("Heart"));
}

void ABossSavarog::BeginPlay()
{
	Super::BeginPlay();

	m_AttackEnd = false;
	m_bInDistance = false;
	m_bAnim = false;
	m_fAtime = 0.f;
	m_iSkillNum = 0;
	m_BossState = EBossState::IDLE;
	SetActorRelativeScale3D(FVector(3, 3, 3)); //크기 조절
	ChangeAnimType(EMonsterAnimType::Idle);
}

void ABossSavarog::ChangeState()
{

}


void ABossSavarog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_bInDistance = FindTarget();
	ChangeState();
	
	//EMonsterAnimType state = m_AnimInstance->GetAnimType();
	switch (m_BossState)
	{
	case EBossState::IDLE:
	{
		if (!m_bAnim)
		{
			PrintViewport(1.f, FColor::Red, "Idle");
			ChangeAnimType(EMonsterAnimType::Idle);
			m_bAnim = true;
		}
		m_fAtime += DeltaTime;

		if (m_bInDistance && m_fAtime > 2.f)
		{
			m_AttackEnd = false;
			m_fAtime = 0.f;
			m_bAnim = false;
			m_BossState = EBossState::TRACE;
		}
		break;
	}
	case EBossState::TRACE:
	{
		if (!m_bAnim)
		{
			PrintViewport(1.f, FColor::Red, "trace");
			if(m_fTarDis >= 400.f) ChangeAnimType(EMonsterAnimType::Run);
			else ChangeAnimType(EMonsterAnimType::Walk);
			m_bAnim = true;
		}
		Trace(DeltaTime);
		m_fAtime += DeltaTime;
		if (m_fAtime > 5.5f)
		{
			m_fAtime = 0.f;
			m_bAnim = false;
			m_BossState = EBossState::IDLE;
		}
		break;
	}
	case EBossState::ATTACK:
	{
		Attack(DeltaTime);
		break;
	}

	case EBossState::HIT_RCV:
		HitRcv(DeltaTime);
		break;
	case EBossState::RUNAWAY:
	{
		if (!m_bAnim)
		{
			PrintViewport(1.f, FColor::Red, "Runaway");
			ChangeAnimType(EMonsterAnimType::Skill);
			m_bAnim = true;
		}
		RunAway(DeltaTime);

		m_fAtime += DeltaTime;
		if (m_fAtime > 2.5f)
		{
			m_fAtime = 0.f;
			m_bAnim = false;
			m_BossState = EBossState::IDLE;
		}
		break;
	}

	case EBossState::DEATH:
		ChangeAnimType(EMonsterAnimType::Death);
		break;
	}
	
}

void ABossSavarog::NormalAttack()
{
	

}

float ABossSavarog::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return Damage;
}


bool ABossSavarog::FindTarget()
{
	APlayerCharacter* Target = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (!Target)
	{
		ChangeAnimType(EMonsterAnimType::Idle);
		return false;
	}

	const FMonsterInfo& MonsterInfo = GetMonsterInfo();

	FVector	MonsterLoc = GetActorLocation();
	FVector	TargetLoc = Target->GetActorLocation();

	MonsterLoc.Z = TargetLoc.Z;

	m_fTarDis = FVector::Distance(MonsterLoc, TargetLoc);
	if (m_fTarDis <= MonsterInfo.TraceDistance)
	{
		return true;
	}
	return false;
}
void ABossSavarog::Trace(float _dt)
{
	APlayerCharacter* Target = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (!Target)
	{
		ChangeAnimType(EMonsterAnimType::Idle);
		return;
	}	
	
	const FMonsterInfo& MonsterInfo = GetMonsterInfo();
	FVector	MonsterLoc = GetActorLocation();
	FVector	TargetLoc = Target->GetActorLocation();
	FVector TarVec = TargetLoc - MonsterLoc;

	//타겟 방향으로 몬스터 회전
	FVector	Dir = TargetLoc - MonsterLoc;
	Dir.Normalize();
	SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));

	float Dis = FVector::Distance(MonsterLoc, TargetLoc);

	if (Dis >= MonsterInfo.AttackDistance)
	{
		TarVec.Normalize();

		MonsterLoc.X += TarVec.X * _dt * MonsterInfo.MoveSpeed;
		MonsterLoc.Y += TarVec.Y * _dt * MonsterInfo.MoveSpeed;

		SetActorLocation(MonsterLoc);
	}
	else
	{
		m_fAtime = 0.f;
		m_bAnim = false;
		m_BossState = EBossState::ATTACK;
	}
}
void ABossSavarog::Attack(float _dt)
{
	switch (m_iSkillNum)
	{
	case 0:
	{
		if (!m_bAnim)
		{
			PrintViewport(1.f, FColor::Red, "attack");
			//ChangeAnimType(EMonsterAnimType::Attack);
			if (!m_AnimInstance->Montage_IsPlaying(m_AttackMontageArray[0]))
			{
				m_AnimInstance->Montage_SetPosition(m_AttackMontageArray[0], 0.f);
				m_AnimInstance->Montage_Play(m_AttackMontageArray[0]);
			}
			m_bAnim = true;
		}

		if (m_AttackEnd)
		{
			m_fAtime = 0.f;
			m_bAnim = false;
			
			if (m_fTarDis <= 200.f)
				m_BossState = EBossState::RUNAWAY;
			else
				m_BossState = EBossState::IDLE;
		}
		break;
	}
	}
}
void ABossSavarog::HitRcv(float _dt)
{
	//몽타주 재생
}
void ABossSavarog::RunAway(float _dt)
{
	APlayerCharacter* Target = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (!Target)
	{
		ChangeAnimType(EMonsterAnimType::Idle);
		return;
	}

	FVector	MonsterLoc = GetActorLocation();
	FVector	TargetLoc = Target->GetActorLocation();
	FVector TarVec = TargetLoc - MonsterLoc;

	TarVec.Normalize();

	MonsterLoc.X += -TarVec.X * _dt * 600.f;
	MonsterLoc.Y += -TarVec.Y * _dt * 600.f;

	SetActorLocation(MonsterLoc);

	//타겟 방향으로 몬스터 회전
	FVector	Dir = TargetLoc - MonsterLoc;
	Dir.Normalize();
	SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
}

void ABossSavarog::Death()
{
	Destroy();
}
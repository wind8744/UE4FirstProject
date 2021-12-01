// Fill out your copyright notice in the Description page of Project Settings.


#include "Wraith.h"
#include "PlayerAnim.h"
#include "../Effect/NormalEffect.h"
//#include "Weapon.h"

AWraith::AWraith()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	WraithAsset(TEXT("SkeletalMesh'/Game/ParagonWraith/Characters/Heroes/Wraith/Skins/LunarOps/Meshes/Wraith_LunarOps.Wraith_LunarOps'"));


	// GetMesh() : CharacterŬ������ ��������ִ� SkeletalMeshComponent�� ������ �Լ��̴�.
	if (WraithAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(WraithAsset.Object);
		m_PlayerMesh = WraithAsset.Object;
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance>	WraithAnimAsset(TEXT("AnimBlueprint'/Game/Player/BPWraithAnim.BPWraithAnim_C'"));

	if (WraithAnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(WraithAnimAsset.Class);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack1Asset(TEXT("AnimMontage'/Game/Player/AMWraithAttack1.AMWraithAttack1'"));

	if (Attack1Asset.Succeeded())
		m_AttackMontageArray.Add(Attack1Asset.Object);

	/*static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill1Asset(TEXT("AnimMontage'/Game/Player/AMGreystoneSkill1.AMGreystoneSkill1'"));

	if (Skill1Asset.Succeeded())
		m_SkillMontageArray.Add(Skill1Asset.Object);

	static ConstructorHelpers::FClassFinder<AActor>	Skill1Class(TEXT("Blueprint'/Game/Player/Greystone/BPGreystoneSkill1.BPGreystoneSkill1_C'"));

	if (Skill1Class.Succeeded())
		m_Skiill1Class = Skill1Class.Class;*/

	m_AttackIndex = 0;

	m_PlayerInfo.Name = TEXT("Wraith");
	m_PlayerInfo.Job = EPlayerJob::Archer;
	m_PlayerInfo.Attack = 120;
	m_PlayerInfo.Armor = 20;
	m_PlayerInfo.HP = 500;
	m_PlayerInfo.HPMax = 500;
	m_PlayerInfo.MP = 100;
	m_PlayerInfo.MPMax = 100;
	m_PlayerInfo.AttackDistance = 200.f;
	m_PlayerInfo.AttackSpeed = 1.f;
	m_PlayerInfo.AttackAngle = 22.5f;
	m_PlayerInfo.MoveSpeed = 600.f;
}

// Called when the game starts or when spawned
void AWraith::BeginPlay()
{
	// Super : �θ�Ŭ������ �ǹ��Ѵ�.
	Super::BeginPlay();
}

// Called every frame
void AWraith::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWraith::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}




void AWraith::Attack()
{
	// 0�� ���� ����� ����� �ȵǰ� �ִٸ� ����� �����ش�.
	if (!m_AnimInstance->Montage_IsPlaying(m_AttackMontageArray[m_AttackIndex]))
	{
		m_AnimInstance->Montage_SetPosition(m_AttackMontageArray[m_AttackIndex], 0.f);
		m_AnimInstance->Montage_Play(m_AttackMontageArray[m_AttackIndex]);
		m_AnimInstance->SetAttackEnable(true);

		m_AttackIndex = (m_AttackIndex + 1) % m_AttackMontageArray.Num();
	}
}

void AWraith::Skill1()
{
	// 0�� ���� ����� ����� �ȵǰ� �ִٸ� ����� �����ش�.
	if (!m_AnimInstance->Montage_IsPlaying(m_SkillMontageArray[0]))
	{
		m_AnimInstance->Montage_SetPosition(m_SkillMontageArray[0], 0.f);
		m_AnimInstance->Montage_Play(m_SkillMontageArray[0]);
	}
}

void AWraith::NormalAttack()
{
	Super::NormalAttack();

	FVector	PlayerLoc = GetActorLocation();
	FVector	Forward = GetActorForwardVector();

	FCollisionQueryParams	params(NAME_None, false, this);

	// ������������ �� Ÿ�ֿ̹� �浹ó���� ���ֵ��� �Ѵ�.
	TArray<FHitResult>	HitResultArray;
	bool Sweep = GetWorld()->SweepMultiByChannel(HitResultArray, PlayerLoc,
		PlayerLoc, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeSphere(m_PlayerInfo.AttackDistance),
		params);

	//LOG(TEXT("Attack : %.5f"), 200.f);
	//LOG(TEXT("TestAttack"));
	//PrintViewport(1.f, FColor::Yellow, TEXT("Attack"));

	TArray<FHitResult>	CollisionArray;
	if (Sweep)
	{
		// 1�� ���浹�� �Ͼ Ÿ�ٰ� ������ ������ �����ش�.
		//for (int32 i = 0; i < HitResultArray.Num(); ++i)
		for (auto& result : HitResultArray)
		{
			FVector	Dir = result.ImpactPoint - GetActorLocation();

			// ������ ���⺤�͸� �������ͷ� �����. �� ������ ���� ���·� üũ�� �ϱ� ���ؼ�
			// ������ üũ����� �ϴµ� ���������� ������ �̿��ϰ� �ȴٸ� �� ���Ͱ� ������
			// �ڻ��� ��Ÿ ���� ������ �� �ִ�.
			// �׷��Ƿ� �̷��� �ڻ��� ��Ÿ�� ��ũ�ڻ����� �̿��Ͽ� ��Ÿ(����)�� �����ϰ�
			// �̸� ���Ͽ� ���� �ȿ� ���������� �Ǵ��� �� �ִ�.
			Dir.Normalize();

			Forward = GetActorForwardVector();
			Forward.Normalize();

			float Dot = FVector::DotProduct(Dir, Forward);
			// Acos�� �̿��ؼ� ������ ���ϸ� �� ���� Radian ������ ������ �ȴ�.
			// �׷��Ƿ� �̸� Degree �� ��ȯ���ְ� �̸� ���ϴ� ������ ����Ѵ�.
			float Angle = FMath::Acos(Dot);
			Angle = FMath::RadiansToDegrees(Angle);

			if (Angle <= m_PlayerInfo.AttackAngle)
				CollisionArray.Add(result);

			else
			{
				// �̰� �ƴ϶�� ������ �̿��ؼ� ���� �浹ü�� �����ϴ��� �Ǵ��Ѵ�.
			}
		}
	}
#if ENABLE_DRAW_DEBUG

	FColor	DrawColor = CollisionArray.Num() > 0 ? FColor::Red : FColor::Green;

	//DrawDebugSphere(GetWorld(), PlayerLoc, m_PlayerInfo.AttackDistance, 20, DrawColor, false, 1.f);
	DrawDebugCone(GetWorld(), PlayerLoc, GetActorForwardVector(), m_PlayerInfo.AttackDistance,
		FMath::DegreesToRadians(m_PlayerInfo.AttackAngle),
		FMath::DegreesToRadians(m_PlayerInfo.AttackAngle), 20,
		DrawColor, false, 1.f);

#endif

	for (auto& result : CollisionArray)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			result.ImpactPoint, result.ImpactNormal.Rotation(), param);

		// �ּ��� �ε��Ѵ�.
		Effect->LoadParticle(TEXT("ParticleSystem'/Game/AdvancedMagicFX12/particles/P_ky_hit_thunder.P_ky_hit_thunder'"));

		// Sound
		Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));


		// �������� �����Ѵ�.
		FDamageEvent	DmgEvent;
		float Damage = result.GetActor()->TakeDamage(m_PlayerInfo.Attack, DmgEvent, GetController(), this);
	}
}

void AWraith::AttackEnd()
{
	Super::AttackEnd();

	m_AttackIndex = 0;
}

void AWraith::UseSkill(int32 Index)
{
	switch (Index)
	{
	case 0:
	{
		/*AWraithSkill1* Skill = GetWorld()->SpawnActor<AWraithSkill1>(m_Skiill1Class,
			GetActorLocation() + GetActorForwardVector() * 100.f,
			GetActorRotation());*/
	}
	break;
	}
}



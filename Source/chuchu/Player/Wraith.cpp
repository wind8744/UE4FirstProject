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


	// GetMesh() : Character클래스에 만들어져있는 SkeletalMeshComponent를 얻어오는 함수이다.
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
	// Super : 부모클래스를 의미한다.
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
	// 0번 공격 모션이 재생이 안되고 있다면 재생을 시켜준다.
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
	// 0번 공격 모션이 재생이 안되고 있다면 재생을 시켜준다.
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

	// 근접공격으로 이 타이밍에 충돌처리를 해주도록 한다.
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
		// 1차 구충돌이 일어난 타겟과 나와의 각도를 구해준다.
		//for (int32 i = 0; i < HitResultArray.Num(); ++i)
		for (auto& result : HitResultArray)
		{
			FVector	Dir = result.ImpactPoint - GetActorLocation();

			// 구해준 방향벡터를 단위벡터로 만든다. 그 이유는 원뿔 형태로 체크를 하기 위해서
			// 각도를 체크해줘야 하는데 단위벡터의 내적을 이용하게 된다면 더 벡터간 각도의
			// 코사인 세타 값을 구해줄 수 있다.
			// 그러므로 이러한 코사인 세타를 아크코사인을 이용하여 세타(각도)로 변경하고
			// 이를 비교하여 원뿔 안에 들어오는지를 판단할 수 있다.
			Dir.Normalize();

			Forward = GetActorForwardVector();
			Forward.Normalize();

			float Dot = FVector::DotProduct(Dir, Forward);
			// Acos를 이용해서 각도를 구하면 이 값은 Radian 값으로 들어오게 된다.
			// 그러므로 이를 Degree 로 변환해주고 이를 비교하는 각도로 사용한다.
			float Angle = FMath::Acos(Dot);
			Angle = FMath::RadiansToDegrees(Angle);

			if (Angle <= m_PlayerInfo.AttackAngle)
				CollisionArray.Add(result);

			else
			{
				// 이게 아니라면 직선을 이용해서 상대방 충돌체를 관통하는지 판단한다.
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

		// 애셋을 로딩한다.
		Effect->LoadParticle(TEXT("ParticleSystem'/Game/AdvancedMagicFX12/particles/P_ky_hit_thunder.P_ky_hit_thunder'"));

		// Sound
		Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));


		// 데미지를 전달한다.
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



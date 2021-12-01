// Fill out your copyright notice in the Description page of Project Settings.


#include "Greystone.h"
#include "PlayerAnim.h"
#include "../Effect/NormalEffect.h"
#include "Weapon.h"
#include "../Effect/GreystoneSkill2Decal.h"
#include "../Effect/GhostTrail.h"

// Sets default values
AGreyStone::AGreyStone()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> GreyStoneAsset(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));

	// ��Ʈ Ʈ����
	// �θ� Ŭ������ �޽��� ��
	if (GreyStoneAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(GreyStoneAsset.Object); 	// GetMesh() : CharacterŬ������ ��������ִ� SkeletalMeshComponent�� ������ �Լ��̴�.
		m_PlayerMesh = GreyStoneAsset.Object; 
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> GreyStoneAnimAsset(TEXT("AnimBlueprint'/Game/Player/BPGreyAnim.BPGreyAnim_C'"));
	if (GreyStoneAnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(GreyStoneAnimAsset.Class);

	// ���� ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack1Asset(TEXT("AnimMontage'/Game/Player/AMGreyStoneAttack1.AMGreyStoneAttack1'"));
	if (Attack1Asset.Succeeded())
		m_AttackMontageArray.Add(Attack1Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack2Asset(TEXT("AnimMontage'/Game/Player/AMGreystoneAttack2.AMGreystoneAttack2'"));

	if (Attack2Asset.Succeeded())
		m_AttackMontageArray.Add(Attack2Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack3Asset(TEXT("AnimMontage'/Game/Player/AMGreystoneAttack3.AMGreystoneAttack3'"));

	if (Attack3Asset.Succeeded())
		m_AttackMontageArray.Add(Attack3Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack4Asset(TEXT("AnimMontage'/Game/Player/AMGreystoneAttack4.AMGreystoneAttack4'"));

	if (Attack4Asset.Succeeded())
		m_AttackMontageArray.Add(Attack4Asset.Object);

	//skill1��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill1Asset(TEXT("AnimMontage'/Game/Player/AMGreyStoneSkill1.AMGreyStoneSkill1'"));
	if (Skill1Asset.Succeeded())
		m_SkillMontageArray.Add(Skill1Asset.Object);
	//sklill2��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill2Asset(TEXT("AnimMontage'/Game/Player/AM_GreystoneSkill2.AM_GreystoneSkill2'"));
	if (Skill2Asset.Succeeded())
		m_SkillMontageArray.Add(Skill2Asset.Object);
	//skill2 fire ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill2FireAsset(TEXT("AnimMontage'/Game/Player/AMGreystoneSkill2Fire.AMGreystoneSkill2Fire'"));
	if (Skill2FireAsset.Succeeded())
		m_Skill2FireMontage = Skill2FireAsset.Object;


	// bp��ų1
	static ConstructorHelpers::FClassFinder<AActor>	Skill1Class(TEXT("Blueprint'/Game/Player/GreyStone/BPGreyStoneSkill1.BPGreyStoneSkill1_C'"));

	if (Skill1Class.Succeeded())
		m_Skiill1Class = Skill1Class.Class;

	//fall reconvery ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	FallRecoveryAsset(TEXT("AnimMontage'/Game/Player/AMGreystoneFallRecovery.AMGreystoneFallRecovery'"));

	if (FallRecoveryAsset.Succeeded())
	{
		m_FallRecoveryMontage = FallRecoveryAsset.Object;

		//ó���� ���� �����ִ� ��Ȱ
		m_FallRecoveryMontage->BlendIn.SetBlendOption(EAlphaBlendOption::Cubic);  //���� �ɼ�
		m_FallRecoveryMontage->BlendIn.SetBlendTime(0.1f);

		m_FallRecoveryMontage->BlendOut.SetBlendOption(EAlphaBlendOption::Cubic); //���� �ɼ�
		m_FallRecoveryMontage->BlendOut.SetBlendTime(0.1f);
	}

	//tail
	static ConstructorHelpers::FObjectFinder<UParticleSystem>	TrailAsset(TEXT("ParticleSystem'/Game/Particle/PSPlayerTrail.PSPlayerTrail'"));

	if (TrailAsset.Succeeded())
		m_Trail->SetTemplate(TrailAsset.Object);

	//m_PlayerInfo.Name = TEXT("Greystone");
	m_PlayerInfo.Job = EPlayerJob::Knight;
	m_PlayerInfo.Attack = 100;
	m_PlayerInfo.Armor = 20;
	m_PlayerInfo.HP = 500;
	m_PlayerInfo.HPMax = 500;
	m_PlayerInfo.MP = 100;
	m_PlayerInfo.MPMax = 100;
	m_PlayerInfo.AttackDistance = 200.f;
	m_PlayerInfo.AttackSpeed = 1.f;
	m_PlayerInfo.AttackAngle = 22.5f;
	m_PlayerInfo.MoveSpeed = 600.f;

	m_AttackIndex = 0;
	m_OnSkill2 = false;

}

// Called when the game starts or when spawned
void AGreyStone::BeginPlay()
{
	Super::BeginPlay();

	// ���ͷ� ���� ���� �����ϱ� ���� �غ�
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	m_Weapon = GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass(),
		FVector::ZeroVector, FRotator::ZeroRotator,
		param);

	//�޽����ٰ� ���� ���� ���͸� ���� ��(���͸� �ٸ� ������ ���ϵ�� ������ִ� ����) ( Attach to Actor�� ���� )
	m_Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, //������� ������ ������ ���̴�
		TEXT("BackWeapon")); //���� �̸�

	//��� ���� �տ� ������ �޾� �ٿ��� ���� ����
	// �׶� �� �� �ֵ��� �Լ��� ������
	//m_Weapon->DetachFromActor 
	
	// � ����޽� ��
	m_Weapon->SetMesh(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Weapon_ForgingHammers/SK_Forging_Mallet_03.SK_Forging_Mallet_03'"));

	m_PlayerInfo.Name = m_PlayerInfo.Name; // TEXT("Greystone"); //����
}

// Called every frame
void AGreyStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ������ ��ų
	// ���Ŀ���� �̿��ؼ� ����ϴ� �� �˻�
	// selectplayercontroller���� ���Ŀ�� �����
	// Ŀ���� ���� �����ؼ� �� Ŀ���� �������� ����� �ؾ��ϹǷ�
	if (m_OnSkill2)
	{
		FHitResult	result;

		bool Hit = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(
			ECollisionChannel::ECC_GameTraceChannel5, //RayLand�� ����̹Ƿ�
			false, result);

		if (Hit)
		{
			m_Skill2Decal->SetActorLocation(result.ImpactPoint);
		}
	}
}

// Called to bind functionality to input
void AGreyStone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//���� Ű(���콺 ����)�� ������ ����
void AGreyStone::Attack()
{

	if (m_OnSkill2) //��ų2�϶���
	{
		m_Skill2Decal->Destroy(); //��Į ����
		m_OnSkill2 = false;

		//���� ������ ���� skill2��Ÿ�� �ִ� ���
		if (!m_AnimInstance->Montage_IsPlaying(m_Skill2FireMontage))
		{
			m_AnimInstance->Montage_SetPosition(m_Skill2FireMontage, 0.f);
			m_AnimInstance->Montage_Play(m_Skill2FireMontage);

			// Ghost Trail On
			OnGhostTrail();
		}
	}

	else
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
	////// 0�� ���� ����� ����� �ȵǰ� �ִٸ� ����� �����ش�.
	//if (!m_AnimInstance->Montage_IsPlaying(m_AttackMontageArray[m_AttackIndex]))
	//{
	//	m_AnimInstance->Montage_SetPosition(m_AttackMontageArray[m_AttackIndex], 0.f);//ani �ð� 0����  �ʱ�ȭ
	//	m_AnimInstance->Montage_Play(m_AttackMontageArray[m_AttackIndex]);
	//	m_AnimInstance->SetAttackEnable(true);

	//	//0 ~ 3���� �ݺ��ϵ��� 
	//	m_AttackIndex = (m_AttackIndex + 1) % m_AttackMontageArray.Num();
	//}
}

void AGreyStone::Skill1()
{
	// 0�� ���� ����� ����� �ȵǰ� �ִٸ� ����� �����ش�.
	if (!m_AnimInstance->Montage_IsPlaying(m_SkillMontageArray[0]))
	{
		m_AnimInstance->Montage_SetPosition(m_SkillMontageArray[0], 0.f);
		m_AnimInstance->Montage_Play(m_SkillMontageArray[0]);
	}
}

void AGreyStone::NormalAttack()
{
	Super::NormalAttack();

	FVector	PlayerLoc = GetActorLocation();
	FVector	Forward = GetActorForwardVector();

	FCollisionQueryParams	params(NAME_None, false, this); //�浹�� ���� �Ķ���͵� 

	// ������������ �� Ÿ�ֿ̹� �浹ó���� ���ֵ��� �Ѵ�.
	TArray<FHitResult>	HitResultArray; //t��� Ÿ������ hit��� �迭�� ������� , �浹�� �� �� �浹�� ������� �����ϴ� ����ü
	//impactpoint�� �ε��� ��ġ normal�� �ε��� ���� 
	bool Sweep = GetWorld()->SweepMultiByChannel(HitResultArray, PlayerLoc, //��Ƽ�� �������� �̱��� �Ѹ��� //�ι�° ���ڴ� �浹 ������	
		PlayerLoc, FQuat::Identity, //���� �Ÿ�, ȸ�������� �⺻ 
		ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeSphere(m_PlayerInfo.AttackDistance), //engineTrace���xxx ,
		params);
	
	//�α� ��� ����
	//LOG(TEXT("Attack : %.5f"), 200.f);
	//LOG(TEXT("TestAttack"));
	PrintViewport(1.f, FColor::Yellow, TEXT("Attack"));


	TArray<FHitResult>	CollisionArray;
	if (Sweep)
	{
		// 1�� ���浹�� �Ͼ Ÿ�ٰ� ������ ������ �����ش�.
		//for (int32 i = 0; i < HitResultArray.Num(); ++i)
		for (auto& result : HitResultArray)
		{
			//result.ImpactPoint = �ε��� ��ġ ����Ʈ 
			FVector	Dir = result.ImpactPoint - GetActorLocation(); //���͸� ���ϴ� ���� ������ ����

			// ������ ���⺤�͸� �������ͷ� �����. �� ������ ���� ���·� üũ�� �ϱ� ���ؼ�
			// ������ üũ����� �ϴµ� ���������� ������ �̿��ϰ� �ȴٸ� �� ���Ͱ� ������
			// �ڻ��� ��Ÿ ���� ������ �� �ִ�.
			// �׷��Ƿ� �̷��� �ڻ��� ��Ÿ�� ��ũ�ڻ����� �̿��Ͽ� ��Ÿ(����)�� �����ϰ�
			// �̸� ���Ͽ� ���� �ȿ� ���������� �Ǵ��� �� �ִ�.
			Dir.Normalize(); //�������ͷ� ����

			Forward = GetActorForwardVector();
			Forward.Normalize();

			float Dot = FVector::DotProduct(Dir, Forward); //���������� ������ �ϸ� �ڽ��� ���� ���´�

			// Acos�� �̿��ؼ� ������ ���ϸ� �� ���� Radian ������ ������ �ȴ�.
			// �׷��Ƿ� �̸� Degree �� ��ȯ���ְ� �̸� ���ϴ� ������ ����Ѵ�.
			float Angle = FMath::Acos(Dot);
			Angle = FMath::RadiansToDegrees(Angle); //��׸� ������ ��ȯ

			if (Angle <= m_PlayerInfo.AttackAngle) //���� ���� �ȿ� ��������
				CollisionArray.Add(result); //���� ���� �ȿ� �����Ƿ� �迭�� �־��ش�.

			else
			{
				// �̰� �ƴ϶�� ������ �̿��ؼ� ���� �浹ü�� �����ϴ��� �Ǵ��Ѵ�.

				// ������ �̿��Ͽ� ��ü�� �����ϴ��� Ȯ���ϴ� ��
				//GetWorld()->LineTraceSingleByChannel()

			}
		}
	}
#if ENABLE_DRAW_DEBUG

	FColor	DrawColor = CollisionArray.Num() > 0 ? FColor::Red : FColor::Green;
	//FColor	DrawColor = Sweep  ? FColor::Red : FColor::Green;

	//DrawDebugSphere(GetWorld(), PlayerLoc, 200.f, 20, DrawColor, false, 1.f); //1�ʵ��� ����ڿ����� �浹ü �׷��� 
	DrawDebugCone(GetWorld(), PlayerLoc, GetActorForwardVector(), m_PlayerInfo.AttackDistance,
		FMath::DegreesToRadians(m_PlayerInfo.AttackAngle),
		FMath::DegreesToRadians(m_PlayerInfo.AttackAngle), 20,
		DrawColor, false, 1.f);
#endif


	for (auto& result : CollisionArray)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // �ε����� �Ⱥε����� ������ �ҷ�������

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			result.ImpactPoint, result.ImpactNormal.Rotation(), param); //��ġ����, ȸ������(�������?:�ε������� ���⺤���� �ݴ����), ���⺤�͸� ȸ�������� �ٲپ���, ������ �ҷ��� �Ķ���� ���� 

		// �ּ��� �ε��Ѵ�.
		Effect->LoadParticle(TEXT("ParticleSystem'/Game/AdvancedMagicFX12/particles/P_ky_hit_thunder.P_ky_hit_thunder'"));

		// Sound
		Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));


		// �������� �����Ѵ�.
		FDamageEvent	DmgEvent;
		//���� ������
		float Damage = result.GetActor()->TakeDamage(m_PlayerInfo.Attack, DmgEvent, GetController(), this); 

	}



}

void AGreyStone::AttackEnd()
{
	Super::AttackEnd();

	m_AttackIndex = 0;
}

void AGreyStone::UseSkill(int32 Index)
{
	switch (Index)
	{
	case 0:
	{
		AGreyStoneSkill1* Skill = GetWorld()->SpawnActor<AGreyStoneSkill1>(m_Skiill1Class,
			GetActorLocation() + GetActorForwardVector() * 100.f,
			GetActorRotation());
	}
	break;
	case 1:
	{
		m_OnSkill2 = true;

		//skill2 ��Į�� ���� ����, effect�� effectdecal �� ��ӹ޾� skill2��Į�� �����

		FActorSpawnParameters	param;

		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//��Į ������� �ҷ���
		m_Skill2Decal = GetWorld()->SpawnActor<AGreystoneSkill2Decal>(AGreystoneSkill2Decal::StaticClass(),
			GetActorLocation(), FRotator::ZeroRotator, param);
	}
	break;
	}
}



void AGreyStone::Skill2()
{
	// 0�� ���� ����� ����� �ȵǰ� �ִٸ� ����� �����ش�.
	if (!m_AnimInstance->Montage_IsPlaying(m_SkillMontageArray[1]))
	{
		m_AnimInstance->Montage_SetPosition(m_SkillMontageArray[1], 0.f);
		m_AnimInstance->Montage_Play(m_SkillMontageArray[1]);

		//

	}

	/*else
	{
		m_AnimInstance->Montage_Stop(0.1f);
		m_AnimInstance->Montage_SetPosition(m_SkillMontageArray[1], 0.f);
		m_AnimInstance->Montage_Play(m_SkillMontageArray[1]);
	}*/
}
void AGreyStone::UseSkillFire(int32 Index)
{
	switch (Index)
	{
	case 0:
		break;
	case 1:
	{
		// ���⼭ ���׿� �߻�!!!!
	}
	break;
	}
}





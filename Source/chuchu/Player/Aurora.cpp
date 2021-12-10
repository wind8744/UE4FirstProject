// Fill out your copyright notice in the Description page of Project Settings.

#include "Aurora.h"
#include "PlayerAnim.h"
#include "../Effect/NormalEffect.h"
#include "Weapon.h"
#include "../Effect/GhostTrail.h"

// Sets default values
AAurora::AAurora()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> AuroraAsset(TEXT("SkeletalMesh'/Game/ParagonAurora/Characters/Heroes/Aurora/Skins/GlacialEmpress/Meshes/Aurora_GlacialEmpress.Aurora_GlacialEmpress'"));


	// �θ� Ŭ������ �޽��� ��
	if (AuroraAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(AuroraAsset.Object); 	// GetMesh() : CharacterŬ������ ��������ִ� SkeletalMeshComponent�� ������ �Լ��̴�.
		m_PlayerMesh = AuroraAsset.Object; 	// ��Ʈ Ʈ����
	}

	// ���ζ� Anim Asset
	static ConstructorHelpers::FClassFinder<UAnimInstance> AuroraAnimAsset(TEXT("AnimBlueprint'/Game/Player/Aurora/BPAuroraAnim.BPAuroraAnim_C'"));
	if (AuroraAnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AuroraAnimAsset.Class);
	
	// fall reconvery ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	FallRecoveryAsset(TEXT("AnimMontage'/Game/Player/Aurora/AMAuroraFallRecovery.AMAuroraFallRecovery'"));

	if (FallRecoveryAsset.Succeeded())
	{
		m_FallRecoveryMontage = FallRecoveryAsset.Object;

		//ó���� ���� �����ִ� ��Ȱ
		m_FallRecoveryMontage->BlendIn.SetBlendOption(EAlphaBlendOption::Cubic);  //���� �ɼ�
		m_FallRecoveryMontage->BlendIn.SetBlendTime(0.1f);

		m_FallRecoveryMontage->BlendOut.SetBlendOption(EAlphaBlendOption::Cubic); //���� �ɼ�
		m_FallRecoveryMontage->BlendOut.SetBlendTime(0.1f);
	}

	//�뽬 ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	DashAsset(TEXT("AnimMontage'/Game/Player/Aurora/AMAuroraDash.AMAuroraDash'"));
	if (DashAsset.Succeeded())
		m_DashMontage = DashAsset.Object;


	// ���� ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack1Asset(TEXT("AnimMontage'/Game/Player/Aurora/AMAuroraAttack1.AMAuroraAttack1'"));
	if (Attack1Asset.Succeeded())
		m_AttackMontageArray.Add(Attack1Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack2Asset(TEXT("AnimMontage'/Game/Player/Aurora/AMAuroraAttack2.AMAuroraAttack2'"));

	if (Attack2Asset.Succeeded())
		m_AttackMontageArray.Add(Attack2Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack3Asset(TEXT("AnimMontage'/Game/Player/Aurora/AMAuroraAttack3.AMAuroraAttack3'"));

	if (Attack3Asset.Succeeded())
		m_AttackMontageArray.Add(Attack3Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack4Asset(TEXT("AnimMontage'/Game/Player/Aurora/AMAuroraAttack4.AMAuroraAttack4'"));

	if (Attack4Asset.Succeeded())
		m_AttackMontageArray.Add(Attack4Asset.Object);

	//skill1��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Skill1Asset(TEXT("AnimMontage'/Game/Player/Aurora/AMAuroraSkill1.AMAuroraSkill1'"));
	if (Skill1Asset.Succeeded())
		m_SkillMontageArray.Add(Skill1Asset.Object);

	//sklill2��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill2Asset(TEXT("AnimMontage'/Game/Player/Aurora/AMAuroraSkill2.AMAuroraSkill2'"));
	if (Skill2Asset.Succeeded())
		m_SkillMontageArray.Add(Skill2Asset.Object);

	//sklill3��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill3Asset(TEXT("AnimMontage'/Game/Player/Aurora/AMAuroraSkill3.AMAuroraSkill3'"));
	if (Skill3Asset.Succeeded())
		m_SkillMontageArray.Add(Skill3Asset.Object);


	// �������Ʈ // * �������Ʈ * ���� �ּ� �ٿ��ֱ� �� _C ���ٿ� �־�� ��� ����
	// ��ų1
	static ConstructorHelpers::FClassFinder<AActor>	Skill1Class(TEXT("Blueprint'/Game/Player/Aurora/BPAuroraSkill1.BPAuroraSkill1_C'")); 

	if (Skill1Class.Succeeded())
		m_Skill1Class = Skill1Class.Class;


	// Trail -> ���׸��� �ٲپ��ָ� ��.
	static ConstructorHelpers::FObjectFinder<UParticleSystem>	TrailAsset(TEXT("ParticleSystem'/Game/Particle/PSPlayerTrail.PSPlayerTrail'"));
	if (TrailAsset.Succeeded())
		m_Trail->SetTemplate(TrailAsset.Object);

	//// Dash Trail
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> DashTrailAsset(TEXT("NiagaraSystem'/Game/BlinkAndDashVFX/VFX_Niagara/NS_Dash_Mana.NS_Dash_Mana'"));
	if (DashTrailAsset.Succeeded())
		m_DashTrail = DashTrailAsset.Object;


	m_AttackIndex = 0;
	m_PlayerInfo.Name = TEXT("Aurora");
	m_PlayerInfo.Job = EPlayerJob::Knight;
	m_PlayerInfo.Attack = 120;
	m_PlayerInfo.Armor = 20;
	m_PlayerInfo.HP = 500;
	m_PlayerInfo.HPMax = 500;
	m_PlayerInfo.MP = 100;
	m_PlayerInfo.MPMax = 100;
	m_PlayerInfo.AttackDistance = 200.f;
	m_PlayerInfo.AttackSpeed = 1.f;
	m_PlayerInfo.AttackAngle = 22.5f;
	m_PlayerInfo.MoveSpeed = 100.f;

	//m_OnSkill2 = false;
}

// Called when the game starts or when spawned
void AAurora::BeginPlay()
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
		TEXT("Sword_Base")); //���� �̸�

	//��� ���� �տ� ������ �޾� �ٿ��� ���� ����
	// �׶� �� �� �ֵ��� �Լ��� ������
	//m_Weapon->DetachFromActor 

	//�տ� ����޽� ��
	m_Weapon->SetMesh(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight'"));

}

// Called every frame
void AAurora::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ������ ��ų
	// ���Ŀ���� �̿��ؼ� ����ϴ� �� �˻�
	// selectplayercontroller���� ���Ŀ�� �����
	// Ŀ���� ���� �����ؼ� �� Ŀ���� �������� ����� �ؾ��ϹǷ�
	//if (m_OnSkill2)
	//{
	//	FHitResult	result;
	//	bool Hit = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(
	//		ECollisionChannel::ECC_GameTraceChannel5, //RayLand�� ����̹Ƿ�
	//		false, result);
	//	if (Hit)
	//	{
	//		m_Skill2Decal->SetActorLocation(result.ImpactPoint);
	//	}
	//}
}

// Called to bind functionality to input
void AAurora::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAurora::Attack()
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

// ��ų Ű�� ���� �� �Ҥ���
void AAurora::Skill1()
{
	// 0�� ���� ����� ����� �ȵǰ� �ִٸ� ����� �����ش�.
	if (!m_AnimInstance->Montage_IsPlaying(m_SkillMontageArray[0]))
	{
		m_AnimInstance->Montage_SetPosition(m_SkillMontageArray[0], 0.f);
		m_AnimInstance->Montage_Play(m_SkillMontageArray[0]);
	}
}

void AAurora::Skill2()
{
	// 0�� ���� ����� ����� �ȵǰ� �ִٸ� ����� �����ش�.
	if (!m_AnimInstance->Montage_IsPlaying(m_SkillMontageArray[1]))
	{
		m_AnimInstance->Montage_SetPosition(m_SkillMontageArray[1], 0.f);
		m_AnimInstance->Montage_Play(m_SkillMontageArray[1]);
	}
}


void AAurora::Skill3()
{
	// 0�� ���� ����� ����� �ȵǰ� �ִٸ� ����� �����ش�.
	if (!m_AnimInstance->Montage_IsPlaying(m_SkillMontageArray[2]))
	{
		m_AnimInstance->Montage_SetPosition(m_SkillMontageArray[2], 0.f);
		m_AnimInstance->Montage_Play(m_SkillMontageArray[2]);

		// Ghost Trail On
		//OnGhostTrail();
	}
}

void AAurora::Dash()
{
	m_AnimInstance->Montage_SetPosition(m_DashMontage, 0.f);
	m_AnimInstance->Montage_Play(m_DashMontage);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (m_DashTrail)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_DashTrail, GetActorLocation(), GetActorForwardVector().Rotation());

	//�񵿱� ���� �ε�
	//ANormalEffect* Effect2 = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
	//	GetActorLocation(), GetActorForwardVector().Rotation(), param);
	//Effect2->LoadNiagaraAsync(TEXT("AuroraDash"));

	//���� ���� �ε�
	//UNiagaraSystem* particlens = LoadObject<UNiagaraSystem>(GetWorld(), (TEXT("NiagaraSystem'/Game/BlinkAndDashVFX/VFX_Niagara/NS_Blink_Psionic.NS_Blink_Psionic'")));
	//if (particlens)
	//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), particlens, GetActorLocation(), GetActorForwardVector().Rotation());
}

void AAurora::NormalAttack()
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

		// ���̾ư��� �ּ��� �ε��Ѵ�.
		Effect->LoadNiagaraAsync(TEXT("AuroraNormalImpact"));

		// Sound
		Effect->LoadSoundAsync(TEXT("HitNormal"));


		// �������� �����Ѵ�.
		FDamageEvent	DmgEvent;
		float Damage = result.GetActor()->TakeDamage(m_PlayerInfo.Attack, DmgEvent, GetController(), this);
	}
}

void AAurora::AttackEnd()
{
	Super::AttackEnd();

	m_AttackIndex = 0;
}

void AAurora::UseSkill(int32 Index)
{
	switch (Index)
	{
	case 0:
	{
		//��ų�� ���� �� ��ųbp�� �����Ǿ�� �ϹǷ� �װ��� ��������� ������ �ְڴٴ� ��
		AAuroraSkill1* Skill = GetWorld()->SpawnActor<AAuroraSkill1>(m_Skill1Class,
			GetActorLocation() + GetActorForwardVector() * 100.f,
			GetActorRotation());
		int a = 1;
		break;
	}
	case 1:
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// effect 1
		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			GetActorLocation(), GetActorRotation(), param);
		//Effect->LoadParticle(TEXT("ParticleSystem'/Game/ParagonAurora/FX/Particles/Abilities/Ultimate/FX/P_Aurora_Frozen_Ground_Water_Ultimate1.P_Aurora_Frozen_Ground_Water_Ultimate1'"));
		Effect->LoadParticleAsync(TEXT("AuroraSkill2A"));

		// effect 2
		ANormalEffect* Effect1 = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			GetActorLocation(), GetActorRotation(), param);
		Effect1->LoadParticleAsync(TEXT("AuroraSkill2B"));
		//Effect1->LoadParticle(TEXT("ParticleSystem'/Game/ParagonAurora/FX/Particles/Abilities/Ultimate/FX/P_Aurora_Ultimate_InitialBlast.P_Aurora_Ultimate_InitialBlast'"));

		// effect 3
		ANormalEffect* Effect2 = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			GetActorLocation(), GetActorRotation(), param);
		Effect2->LoadParticleAsync(TEXT("AuroraSkill2C"));
		//Effect2->LoadParticle(TEXT("ParticleSystem'/Game/ParagonAurora/FX/Particles/Abilities/Freeze/FX/P_Aurora_Freeze_Whrilwind.P_Aurora_Freeze_Whrilwind'"));

		// sound
		//Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));

		AuroraSkill2();
		
		break;

	}
	// skill 3 ������ ��
	case 2:
	{
		break;
	}

	break;
	}
}


void AAurora::AuroraSkill2()
{
	// �浹ü
	FHitResult result2;
	FVector	PlayerLoc = GetActorLocation();
	FVector	Forward = GetActorForwardVector();

	FCollisionQueryParams	params(NAME_None, false, this); //�浹�� ���� �Ķ���͵� 

	// ������������ �� Ÿ�ֿ̹� �浹ó���� ���ֵ��� �Ѵ�.
	TArray<FHitResult>	HitResultArray; //t��� Ÿ������ hit��� �迭�� ������� , �浹�� �� �� �浹�� ������� �����ϴ� ����ü
	//impactpoint�� �ε��� ��ġ normal�� �ε��� ���� 

	FVector AttackBox;
	AttackBox.X = 300.f;
	AttackBox.Y = 300.f;
	AttackBox.Z = 100.f;

	bool Sweep = GetWorld()->SweepMultiByChannel(HitResultArray, PlayerLoc, //��Ƽ�� �������� �̱��� �Ѹ��� //�ι�° ���ڴ� �浹 ������	
		PlayerLoc, FQuat::Identity, //���� �Ÿ�, ȸ�������� �⺻ 
		ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeBox(AttackBox),//MakeSphere(m_PlayerInfo.AttackDistance), //engineTrace���xxx ,
		params);


#if ENABLE_DRAW_DEBUG
	PrintViewport(1.f, FColor::Yellow, TEXT("Attack"));
	FColor	DrawColor = HitResultArray.Num() > 0 ? FColor::Red : FColor::Green;
	DrawDebugBox(GetWorld(), PlayerLoc, AttackBox, DrawColor, false, 1.f);
#endif


	for (auto& result : HitResultArray)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // �ε����� �Ⱥε����� ������ �ҷ�������

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			result.ImpactPoint, result.ImpactNormal.Rotation(), param); //��ġ����, ȸ������(�������?:�ε������� ���⺤���� �ݴ����), ���⺤�͸� ȸ�������� �ٲپ���, ������ �ҷ��� �Ķ���� ���� 

		// �ּ��� �ε��Ѵ�.
		Effect->LoadParticle(TEXT("ParticleSystem'/Game/ParagonAurora/FX/Particles/Abilities/Freeze/FX/P_Aurora_Freeze_Rooted.P_Aurora_Freeze_Rooted'"));

		// Sound
		Effect->LoadSoundAsync(TEXT("HitNormal"));
		//Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));

		// �������� �����Ѵ�.
		FDamageEvent	DmgEvent;
		//���� ������
		float Damage = result.GetActor()->TakeDamage(m_PlayerInfo.Attack, DmgEvent, GetController(), this);

	}


	// �з�����

}
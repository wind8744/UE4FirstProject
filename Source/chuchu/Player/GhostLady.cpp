// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostLady.h"
#include "PlayerAnim.h"
#include "../Effect/NormalEffect.h"
#include "Weapon.h"
#include "../Effect/GhostTrail.h"
#include "../Monster/Monster.h"

AGhostLady::AGhostLady()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> 
		GhostLadyAsset(TEXT("SkeletalMesh'/Game/GhostLady_S2/Meshes/Characters/Combines/SK_GhostLadyS2_A.SK_GhostLadyS2_A'"));

	if (GhostLadyAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(GhostLadyAsset.Object);
	}

	// Anim Asset
	static ConstructorHelpers::FClassFinder<UAnimInstance> GhostAnimAsset(TEXT("AnimBlueprint'/Game/Player/GhostLady/BPGhostLadyAnim.BPGhostLadyAnim_C'"));
	if (GhostAnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(GhostAnimAsset.Class);

	//몽타주 (Attack)
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack1Asset(TEXT("AnimMontage'/Game/Player/GhostLady/Attack/AMGhostLadyAttack1.AMGhostLadyAttack1'"));
	if (Attack1Asset.Succeeded()) 
		m_AttackMontageArray.Add(Attack1Asset.Object);
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack2Asset(TEXT("AnimMontage'/Game/Player/GhostLady/Attack/AMGhostLadyAttack2.AMGhostLadyAttack2'"));
	if (Attack2Asset.Succeeded())
		m_AttackMontageArray.Add(Attack2Asset.Object);
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack3Asset(TEXT("AnimMontage'/Game/Player/GhostLady/Attack/AMGhostLadyAttack3.AMGhostLadyAttack3'"));
	if (Attack3Asset.Succeeded())
		m_AttackMontageArray.Add(Attack3Asset.Object);

	//몽타주 (Skill)
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Skill1Asset(TEXT("AnimMontage'/Game/Player/GhostLady/Skill/AMGhostLadySkill1.AMGhostLadySkill1'"));
	if (Skill1Asset.Succeeded())
		m_SkillMontageArray.Add(Skill1Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Skill2Asset(TEXT("AnimMontage'/Game/Player/GhostLady/Skill/AMGhostLadySkill2.AMGhostLadySkill2'"));
	if (Skill2Asset.Succeeded())
		m_SkillMontageArray.Add(Skill2Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Skill3Asset(TEXT("AnimMontage'/Game/Player/GhostLady/Skill/AMGhostLadySkill3.AMGhostLadySkill3'"));
	if (Skill3Asset.Succeeded())
		m_SkillMontageArray.Add(Skill3Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Skill4Asset(TEXT("AnimMontage'/Game/Player/GhostLady/Skill/AMGhostLadySkill4.AMGhostLadySkill4'"));
	if (Skill4Asset.Succeeded())
		m_SkillMontageArray.Add(Skill4Asset.Object);

	//fall reconvery 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	FallRecoveryAsset(TEXT("AnimMontage'/Game/Player/GhostLady/AMGhostLadyFallRecovery.AMGhostLadyFallRecovery'"));

	if (FallRecoveryAsset.Succeeded())
	{
		m_FallRecoveryMontage = FallRecoveryAsset.Object;

		//처음과 끝을 섞어주는 역활
		m_FallRecoveryMontage->BlendIn.SetBlendOption(EAlphaBlendOption::Cubic);  //블랜딩 옵션
		m_FallRecoveryMontage->BlendIn.SetBlendTime(0.1f);

		m_FallRecoveryMontage->BlendOut.SetBlendOption(EAlphaBlendOption::Cubic); //블랜딩 옵션
		m_FallRecoveryMontage->BlendOut.SetBlendTime(0.1f);
	}

	// Avoid 몽ㅌㅏ주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Avoid1Asset(TEXT("AnimMontage'/Game/Player/GhostLady/Avoid/AMGhostLadyAvoid1.AMGhostLadyAvoid1'"));
	if (Avoid1Asset.Succeeded())
		m_ArrayAvoidMontage.Add(Avoid1Asset.Object);
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Avoid2Asset(TEXT("AnimMontage'/Game/Player/GhostLady/Avoid/AMGhostLadyAvoid1_2.AMGhostLadyAvoid1_2'"));
	if (Avoid2Asset.Succeeded())
		m_ArrayAvoidMontage.Add(Avoid2Asset.Object);
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Avoid3Asset(TEXT("AnimMontage'/Game/Player/GhostLady/Avoid/AMGhostLadyAvoid1_3.AMGhostLadyAvoid1_3'"));
	if (Avoid3Asset.Succeeded())
		m_ArrayAvoidMontage.Add(Avoid3Asset.Object);
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Avoid4Asset(TEXT("AnimMontage'/Game/Player/GhostLady/Avoid/AMGhostLadyAvoid1_4.AMGhostLadyAvoid1_4'"));
	if (Avoid4Asset.Succeeded())
		m_ArrayAvoidMontage.Add(Avoid4Asset.Object);

	// Trail -> 메테리얼 바꾸어주면 됨.
	static ConstructorHelpers::FObjectFinder<UParticleSystem>	TrailAsset(TEXT("ParticleSystem'/Game/Particle/PSPlayerTrail.PSPlayerTrail'"));
	if (TrailAsset.Succeeded())
		m_Trail->SetTemplate(TrailAsset.Object);

	//// Dash Trail
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> DashTrailAsset(TEXT("NiagaraSystem'/Game/BlinkAndDashVFX/VFX_Niagara/NS_Dash_Vampire.NS_Dash_Vampire'"));
	if (DashTrailAsset.Succeeded())
		m_DashTrail = DashTrailAsset.Object;

	//Equip

	//헬멧
	m_Helmet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Helmet"));
	m_Helmet->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HAIR")); //소켓 이름

	//하의
	m_BotBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BotBody"));
	m_BotBody->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	//상의
	m_TopBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TopBody"));
	m_TopBody->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);// , TEXT("ROOT"));

	//장갑
	m_Hand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hand"));
	m_Hand->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	//시발
	m_Boots = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Boots"));
	m_Boots->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	m_Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	m_Hair->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		GhostLadyHairAsset(TEXT("SkeletalMesh'/Game/GhostLady_S2/Meshes/Characters/Separates/Hairs/SK_LongHair.SK_LongHair'"));
	m_Hair->SetSkeletalMesh(GhostLadyHairAsset.Object);
	//m_Hair->SetRelativeLocation(FVector(0.f, 4.f, -166.f));


	//칼
	m_Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword")); //	
	m_Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SWORD"));   // 블렌딩 때 소켓 안움직임
	FQuat rot; rot.Y = 0.f;
	m_Sword->SetRelativeRotation(rot);
	//m_Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WEAPON_R"));

	//ConstructorHelpers::FObjectFinder<UStaticMesh>
	//	GhostSwordAsset(TEXT("StaticMesh'/Game/GreatSword/GreatSword/Weapon/GreatSword_02.GreatSword_02'"));
	//m_Sword->SetStaticMesh(GhostSwordAsset.Object); //**
	
	//칼 충돌체 캡슐
	m_WeaponCollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollisionCapsule"));
	m_WeaponCollisionCapsule->AttachToComponent(m_Sword, FAttachmentTransformRules::KeepRelativeTransform, TEXT("SWORD"));
	rot.Y = 90.f;
	m_WeaponCollisionCapsule->SetRelativeLocationAndRotation(FVector(100.f, 0.f, 0.f),rot);
	m_WeaponCollisionCapsule->SetRelativeScale3D(FVector(1.f, 1.f, 2.25f));
	m_WeaponCollisionCapsule->SetCollisionProfileName(("WeaponCollision"));
	m_WeaponCollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AGhostLady::OnComponentBeginOverlapWeapon);
	//Equip ==================== end

	m_DidEquipWeapon = false;
	m_AttackIndex = 0;
	m_PlayerInfo.Name = TEXT("GHostLady");
	m_PlayerInfo.Job = EPlayerJob::Knight;
	m_PlayerInfo.Attack = 120;
	m_PlayerInfo.Armor = 20;
	m_PlayerInfo.HP = 500;
	m_PlayerInfo.HPMax = 500;
	m_PlayerInfo.MP = 100;
	m_PlayerInfo.MPMax = 100;
	m_PlayerInfo.AttackDistance = 400.f;
	m_PlayerInfo.AttackSpeed = 1.f;
	m_PlayerInfo.AttackAngle = 22.5f;
	m_PlayerInfo.MoveSpeed = 100.f;
}

// Called when the game starts or when spawned
void AGhostLady::BeginPlay()
{
	// Super : 부모클래스를 의미한다.
	Super::BeginPlay();

}

// Called every frame
void AGhostLady::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGhostLady::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGhostLady::Dash()
{
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//if (m_DashTrail)
	//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_DashTrail, GetActorLocation(), GetActorForwardVector().Rotation());

	FVector ActorVelovity = GetVelocity();
	ActorVelovity.Normalize();

	if (m_DashTrail)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_DashTrail, GetActorLocation(), ActorVelovity.Rotation());


	//비동기 에셋 로딩
	//ANormalEffect* Effect2 = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
	//	GetActorLocation(), GetActorForwardVector().Rotation(), param);
	//Effect2->LoadNiagaraAsync(TEXT("AuroraDash"));

	//동기 에셋 로딩
	//UNiagaraSystem* particlens = LoadObject<UNiagaraSystem>(GetWorld(), (TEXT("NiagaraSystem'/Game/BlinkAndDashVFX/VFX_Niagara/NS_Blink_Psionic.NS_Blink_Psionic'")));
	//if (particlens)
	//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), particlens, GetActorLocation(), GetActorForwardVector().Rotation());
}

void AGhostLady::Attack()
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

void AGhostLady::OnComponentBeginOverlapWeapon(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMonster* Mon = Cast<AMonster>(OtherActor);
	if (Mon)
	{
		// Effect
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			SweepResult.ImpactPoint, SweepResult.ImpactNormal.Rotation(), param);

		// 나이아가라 애셋을 로딩한다.
		Effect->LoadNiagaraAsync(TEXT("GhostLadySlash")); //GhostLadySlash

		// Sound
		//Effect->LoadSoundAsync(TEXT("HitNormal"));

		// 데미지를 전달한다.
		FDamageEvent	DmgEvent;
		float Damage = SweepResult.GetActor()->TakeDamage(m_PlayerInfo.Attack, DmgEvent, GetController(), this);
	}
}
void AGhostLady::NormalAttack()
{
	Super::NormalAttack();

	//========
	//캡슐충돌
	//========
	m_WeaponCollisionCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); //공격이 시작될 때 콜리전 활성화
}

void AGhostLady::AttackEnd()
{
	Super::AttackEnd();

	m_WeaponCollisionCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_AttackIndex = 0;
}

void AGhostLady::EquipItem(EEquipType EquipmentType, const FString& EquipmentPath)
{
	switch (EquipmentType)
	{
	case EEquipType::HAIR:
	{
		USkeletalMesh* asset1 = LoadObject<USkeletalMesh>(nullptr, *EquipmentPath);
		m_Hair->SetSkeletalMesh(asset1);
		break;
	}
	case EEquipType::TOPBODY:
	{
		USkeletalMesh* asset1 = LoadObject<USkeletalMesh>(nullptr, *EquipmentPath);
		m_TopBody->SetSkeletalMesh(asset1);
		break;
	}
	case EEquipType::BOTBODY:
	{
		USkeletalMesh* asset1 = LoadObject<USkeletalMesh>(nullptr, *EquipmentPath);
		m_BotBody->SetSkeletalMesh(asset1);
		break;
	}
	case EEquipType::HAND:
	{
		USkeletalMesh* asset1 = LoadObject<USkeletalMesh>(nullptr, *EquipmentPath);
		m_Hand->SetSkeletalMesh(asset1);
		break;
	}
	case EEquipType::BOOTS:
	{
		USkeletalMesh* asset1 = LoadObject<USkeletalMesh>(nullptr, *EquipmentPath);
		m_Boots->SetSkeletalMesh(asset1);
		break;
	}
	case EEquipType::WEAPON:
	{
		UStaticMesh* weapon = LoadObject<UStaticMesh>(nullptr, *EquipmentPath); //set weapon
		m_Sword->SetStaticMesh(weapon);
		m_DidEquipWeapon = true;
		break;
	}

	}

}

void AGhostLady::RemoveItem(EEquipType EquipmentType)
{
	switch (EquipmentType)
	{
	case EEquipType::HAIR:
	{
		m_Hair->SetSkeletalMesh(nullptr);
		break;
	}
	case EEquipType::TOPBODY:
	{
		m_TopBody->SetSkeletalMesh(nullptr);
		break;
	}
	case EEquipType::BOTBODY:
	{
		m_BotBody->SetSkeletalMesh(nullptr);
		break;
	}
	case EEquipType::HAND:
	{
		m_Hand->SetSkeletalMesh(nullptr);
		break;
	}
	case EEquipType::BOOTS:
	{
		m_Boots->SetSkeletalMesh(nullptr);
		break;
	}
	case EEquipType::WEAPON:
	{
		m_Sword->SetStaticMesh(nullptr); //무기 벗음
		InitWeaponSocket(); // socket, anim 초기화
		m_DidEquipWeapon = false;
		break;
	}

	}
}

void AGhostLady::UseItem()
{
}

void AGhostLady::ChangeWeaponSocket()
{
	m_Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WEAPON_R")); //블랜딩 때 Sword소켓이 안움직이므로 다른 소켓으로 바꾸어줌
}

void AGhostLady::InitWeaponSocket()
{
	m_Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SWORD"));
	m_AnimInstance->InitWeaponAnimPose();
}


void AGhostLady::Skill1() // 바꾸기
{
	if (!m_AnimInstance->Montage_IsPlaying(m_SkillMontageArray[0]))
	{
		m_AnimInstance->Montage_SetPosition(m_SkillMontageArray[0], 0.f);
		m_AnimInstance->Montage_Play(m_SkillMontageArray[0]);
	}
}
void AGhostLady::Skill2()
{
	if (!m_AnimInstance->Montage_IsPlaying(m_SkillMontageArray[1]))
	{
		m_AnimInstance->Montage_SetPosition(m_SkillMontageArray[1], 0.f);
		m_AnimInstance->Montage_Play(m_SkillMontageArray[1]);
	}
}
void AGhostLady::Skill3()
{
	if (!m_AnimInstance->Montage_IsPlaying(m_SkillMontageArray[2]))
	{
		m_AnimInstance->Montage_SetPosition(m_SkillMontageArray[2], 0.f);
		m_AnimInstance->Montage_Play(m_SkillMontageArray[2]);
	}
}
void AGhostLady::Skill4()
{
	if (!m_AnimInstance->Montage_IsPlaying(m_SkillMontageArray[3]))
	{
		m_AnimInstance->Montage_SetPosition(m_SkillMontageArray[3], 0.f);
		m_AnimInstance->Montage_Play(m_SkillMontageArray[3]);
	}
}
void AGhostLady::UseSkill()
{
	m_WeaponCollisionCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); //공격이 시작될 때 콜리전 활성화

	switch (m_PushedSkillIdx)
	{
		case 1:
		{
			m_AnimInstance->ChangeAnimType(EPlayerAnimType::Skill);
			break;
		}
		case 2: //재사용 대기시간
		{
			m_AnimInstance->ChangeAnimType(EPlayerAnimType::Skill);
			LaunchCharacter(FVector(m_Camera->GetForwardVector().X, m_Camera->GetForwardVector().Y, 0.5f).GetSafeNormal() * 1000.f, true, true);//카메라 위아래로 움직짐 방지, 지
			//GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayerCharacter::StopDashing, m_DashStop, false); //dashstop시간 이후 함수 호출
			break;
		}
		case 3: //키다운
		{
			m_AnimInstance->ChangeAnimType(EPlayerAnimType::Skill);
			break;
		}
		case 4: //스택 
		{
			m_AnimInstance->ChangeAnimType(EPlayerAnimType::Skill);
			break;
		}
		default:
			break;
	}

}
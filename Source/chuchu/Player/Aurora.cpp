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


	// 부모 클래스의 메쉬에 셋
	if (AuroraAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(AuroraAsset.Object); 	// GetMesh() : Character클래스에 만들어져있는 SkeletalMeshComponent를 얻어오는 함수이다.
		m_PlayerMesh = AuroraAsset.Object; 	// 고스트 트레일
	}

	// 오로라 Anim Asset
	static ConstructorHelpers::FClassFinder<UAnimInstance> AuroraAnimAsset(TEXT("AnimBlueprint'/Game/Player/Aurora/BPAuroraAnim.BPAuroraAnim_C'"));
	if (AuroraAnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AuroraAnimAsset.Class);
	
	// fall reconvery 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	FallRecoveryAsset(TEXT("AnimMontage'/Game/Player/Aurora/AMAuroraFallRecovery.AMAuroraFallRecovery'"));

	if (FallRecoveryAsset.Succeeded())
	{
		m_FallRecoveryMontage = FallRecoveryAsset.Object;

		//처음과 끝을 섞어주는 역활
		m_FallRecoveryMontage->BlendIn.SetBlendOption(EAlphaBlendOption::Cubic);  //블랜딩 옵션
		m_FallRecoveryMontage->BlendIn.SetBlendTime(0.1f);

		m_FallRecoveryMontage->BlendOut.SetBlendOption(EAlphaBlendOption::Cubic); //블랜딩 옵션
		m_FallRecoveryMontage->BlendOut.SetBlendTime(0.1f);
	}

	//대쉬 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	DashAsset(TEXT("AnimMontage'/Game/Player/Aurora/AMAuroraDash.AMAuroraDash'"));
	if (DashAsset.Succeeded())
		m_DashMontage = DashAsset.Object;


	// 어택 몽타주
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

	//skill1몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Skill1Asset(TEXT("AnimMontage'/Game/Player/Aurora/AMAuroraSkill1.AMAuroraSkill1'"));
	if (Skill1Asset.Succeeded())
		m_SkillMontageArray.Add(Skill1Asset.Object);

	//sklill2몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill2Asset(TEXT("AnimMontage'/Game/Player/Aurora/AMAuroraSkill2.AMAuroraSkill2'"));
	if (Skill2Asset.Succeeded())
		m_SkillMontageArray.Add(Skill2Asset.Object);

	//sklill3몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill3Asset(TEXT("AnimMontage'/Game/Player/Aurora/AMAuroraSkill3.AMAuroraSkill3'"));
	if (Skill3Asset.Succeeded())
		m_SkillMontageArray.Add(Skill3Asset.Object);


	// 블루프린트 // * 블루프린트 * 사용시 주소 붙여넣기 후 _C 덧붙여 주어야 사용 가능
	// 스킬1
	static ConstructorHelpers::FClassFinder<AActor>	Skill1Class(TEXT("Blueprint'/Game/Player/Aurora/BPAuroraSkill1.BPAuroraSkill1_C'")); 

	if (Skill1Class.Succeeded())
		m_Skill1Class = Skill1Class.Class;


	// Trail -> 메테리얼 바꾸어주면 됨.
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

	// 액터로 만든 무기 장착하기 위한 준비
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	m_Weapon = GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass(),
		FVector::ZeroVector, FRotator::ZeroRotator,
		param);

	//메쉬에다가 새로 만든 액터를 붙일 것(액터를 다른 액터의 차일드로 만들어주는 개념) ( Attach to Actor도 있음 )
	m_Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, //상대적인 정보를 유지할 것이다
		TEXT("Sword_Base")); //소켓 이름

	//등에서 떼서 손에 소켓을 달아 붙여줄 수도 있음
	// 그때 땔 수 있도록 함수를 제공함
	//m_Weapon->DetachFromActor 

	//손에 무기메쉬 셋
	m_Weapon->SetMesh(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight'"));

}

// Called every frame
void AAurora::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 마법진 스킬
	// 언더커서를 이용해서 통과하는 것 검사
	// selectplayercontroller에서 언더커서 갖고옴
	// 커서를 꼐속 인지해서 그 커서로 마법진이 생기게 해야하므로
	//if (m_OnSkill2)
	//{
	//	FHitResult	result;
	//	bool Hit = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(
	//		ECollisionChannel::ECC_GameTraceChannel5, //RayLand에 쏠것이므로
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
	// 0번 공격 모션이 재생이 안되고 있다면 재생을 시켜준다.
	if (!m_AnimInstance->Montage_IsPlaying(m_AttackMontageArray[m_AttackIndex]))
	{
		m_AnimInstance->Montage_SetPosition(m_AttackMontageArray[m_AttackIndex], 0.f);
		m_AnimInstance->Montage_Play(m_AttackMontageArray[m_AttackIndex]);
		m_AnimInstance->SetAttackEnable(true);

		m_AttackIndex = (m_AttackIndex + 1) % m_AttackMontageArray.Num();
	}
}

// 스킬 키를 누를 때 불ㄹ미
void AAurora::Skill1()
{
	// 0번 공격 모션이 재생이 안되고 있다면 재생을 시켜준다.
	if (!m_AnimInstance->Montage_IsPlaying(m_SkillMontageArray[0]))
	{
		m_AnimInstance->Montage_SetPosition(m_SkillMontageArray[0], 0.f);
		m_AnimInstance->Montage_Play(m_SkillMontageArray[0]);
	}
}

void AAurora::Skill2()
{
	// 0번 공격 모션이 재생이 안되고 있다면 재생을 시켜준다.
	if (!m_AnimInstance->Montage_IsPlaying(m_SkillMontageArray[1]))
	{
		m_AnimInstance->Montage_SetPosition(m_SkillMontageArray[1], 0.f);
		m_AnimInstance->Montage_Play(m_SkillMontageArray[1]);
	}
}


void AAurora::Skill3()
{
	// 0번 공격 모션이 재생이 안되고 있다면 재생을 시켜준다.
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

	//비동기 에셋 로딩
	//ANormalEffect* Effect2 = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
	//	GetActorLocation(), GetActorForwardVector().Rotation(), param);
	//Effect2->LoadNiagaraAsync(TEXT("AuroraDash"));

	//동기 에셋 로딩
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

		// 나이아가라 애셋을 로딩한다.
		Effect->LoadNiagaraAsync(TEXT("AuroraNormalImpact"));

		// Sound
		Effect->LoadSoundAsync(TEXT("HitNormal"));


		// 데미지를 전달한다.
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
		//스킬이 사용될 때 스킬bp가 생성되어야 하므로 그것을 멤버변수로 가지고 있겠다는 것
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
	// skill 3 만들어야 함
	case 2:
	{
		break;
	}

	break;
	}
}


void AAurora::AuroraSkill2()
{
	// 충돌체
	FHitResult result2;
	FVector	PlayerLoc = GetActorLocation();
	FVector	Forward = GetActorForwardVector();

	FCollisionQueryParams	params(NAME_None, false, this); //충돌을 위한 파라미터들 

	// 근접공격으로 이 타이밍에 충돌처리를 해주도록 한다.
	TArray<FHitResult>	HitResultArray; //t어래이 타입으로 hit결과 배열을 만들어줌 , 충돌을 한 뒤 충돌의 결과값을 저장하는 구조체
	//impactpoint는 부딪힌 위치 normal은 부딪힌 방향 

	FVector AttackBox;
	AttackBox.X = 300.f;
	AttackBox.Y = 300.f;
	AttackBox.Z = 100.f;

	bool Sweep = GetWorld()->SweepMultiByChannel(HitResultArray, PlayerLoc, //멅티는 여러마리 싱글은 한마리 //두번째 인자는 충돌 시작점	
		PlayerLoc, FQuat::Identity, //공격 거리, 회전정보가 기본 
		ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeBox(AttackBox),//MakeSphere(m_PlayerInfo.AttackDistance), //engineTrace사용xxx ,
		params);


#if ENABLE_DRAW_DEBUG
	PrintViewport(1.f, FColor::Yellow, TEXT("Attack"));
	FColor	DrawColor = HitResultArray.Num() > 0 ? FColor::Red : FColor::Green;
	DrawDebugBox(GetWorld(), PlayerLoc, AttackBox, DrawColor, false, 1.f);
#endif


	for (auto& result : HitResultArray)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // 부딪히던 안부딪히던 무조건 불러오도록

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			result.ImpactPoint, result.ImpactNormal.Rotation(), param); //위치정보, 회전정보(어느각도?:부딪혔을때 방향벡터의 반대방향), 방향벡터를 회전정보로 바꾸어줌, 위에서 불러온 파라미터 정보 

		// 애셋을 로딩한다.
		Effect->LoadParticle(TEXT("ParticleSystem'/Game/ParagonAurora/FX/Particles/Abilities/Freeze/FX/P_Aurora_Freeze_Rooted.P_Aurora_Freeze_Rooted'"));

		// Sound
		Effect->LoadSoundAsync(TEXT("HitNormal"));
		//Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));

		// 데미지를 전달한다.
		FDamageEvent	DmgEvent;
		//최종 데미지
		float Damage = result.GetActor()->TakeDamage(m_PlayerInfo.Attack, DmgEvent, GetController(), this);

	}


	// 밀려나기

}
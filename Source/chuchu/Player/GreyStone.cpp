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

	// 고스트 트레일
	// 부모 클래스의 메쉬에 셋
	if (GreyStoneAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(GreyStoneAsset.Object); 	// GetMesh() : Character클래스에 만들어져있는 SkeletalMeshComponent를 얻어오는 함수이다.
		m_PlayerMesh = GreyStoneAsset.Object; 
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> GreyStoneAnimAsset(TEXT("AnimBlueprint'/Game/Player/BPGreyAnim.BPGreyAnim_C'"));
	if (GreyStoneAnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(GreyStoneAnimAsset.Class);

	// 어택 몽타주
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

	//skill1몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill1Asset(TEXT("AnimMontage'/Game/Player/AMGreyStoneSkill1.AMGreyStoneSkill1'"));
	if (Skill1Asset.Succeeded())
		m_SkillMontageArray.Add(Skill1Asset.Object);
	//sklill2몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill2Asset(TEXT("AnimMontage'/Game/Player/AM_GreystoneSkill2.AM_GreystoneSkill2'"));
	if (Skill2Asset.Succeeded())
		m_SkillMontageArray.Add(Skill2Asset.Object);
	//skill2 fire 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill2FireAsset(TEXT("AnimMontage'/Game/Player/AMGreystoneSkill2Fire.AMGreystoneSkill2Fire'"));
	if (Skill2FireAsset.Succeeded())
		m_Skill2FireMontage = Skill2FireAsset.Object;


	// bp스킬1
	static ConstructorHelpers::FClassFinder<AActor>	Skill1Class(TEXT("Blueprint'/Game/Player/GreyStone/BPGreyStoneSkill1.BPGreyStoneSkill1_C'"));

	if (Skill1Class.Succeeded())
		m_Skiill1Class = Skill1Class.Class;

	//fall reconvery 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	FallRecoveryAsset(TEXT("AnimMontage'/Game/Player/AMGreystoneFallRecovery.AMGreystoneFallRecovery'"));

	if (FallRecoveryAsset.Succeeded())
	{
		m_FallRecoveryMontage = FallRecoveryAsset.Object;

		//처음과 끝을 섞어주는 역활
		m_FallRecoveryMontage->BlendIn.SetBlendOption(EAlphaBlendOption::Cubic);  //블랜딩 옵션
		m_FallRecoveryMontage->BlendIn.SetBlendTime(0.1f);

		m_FallRecoveryMontage->BlendOut.SetBlendOption(EAlphaBlendOption::Cubic); //블랜딩 옵션
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

	// 액터로 만든 무기 장착하기 위한 준비
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	m_Weapon = GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass(),
		FVector::ZeroVector, FRotator::ZeroRotator,
		param);

	//메쉬에다가 새로 만든 액터를 붙일 것(액터를 다른 액터의 차일드로 만들어주는 개념) ( Attach to Actor도 있음 )
	m_Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, //상대적인 정보를 유지할 것이다
		TEXT("BackWeapon")); //소켓 이름

	//등에서 떼서 손에 소켓을 달아 붙여줄 수도 있음
	// 그때 땔 수 있도록 함수를 제공함
	//m_Weapon->DetachFromActor 
	
	// 등에 무기메쉬 셋
	m_Weapon->SetMesh(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Weapon_ForgingHammers/SK_Forging_Mallet_03.SK_Forging_Mallet_03'"));

	m_PlayerInfo.Name = m_PlayerInfo.Name; // TEXT("Greystone"); //내거
}

// Called every frame
void AGreyStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 마법진 스킬
	// 언더커서를 이용해서 통과하는 것 검사
	// selectplayercontroller에서 언더커서 갖고옴
	// 커서를 꼐속 인지해서 그 커서로 마법진이 생기게 해야하므로
	if (m_OnSkill2)
	{
		FHitResult	result;

		bool Hit = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(
			ECollisionChannel::ECC_GameTraceChannel5, //RayLand에 쏠것이므로
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

//어택 키(마우스 왼쪽)가 들어오는 순간
void AGreyStone::Attack()
{

	if (m_OnSkill2) //스킬2일때만
	{
		m_Skill2Decal->Destroy(); //데칼 제거
		m_OnSkill2 = false;

		//공격 누르는 순간 skill2몽타주 애니 재생
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
		// 0번 공격 모션이 재생이 안되고 있다면 재생을 시켜준다.
		if (!m_AnimInstance->Montage_IsPlaying(m_AttackMontageArray[m_AttackIndex]))
		{
			m_AnimInstance->Montage_SetPosition(m_AttackMontageArray[m_AttackIndex], 0.f);
			m_AnimInstance->Montage_Play(m_AttackMontageArray[m_AttackIndex]);
			m_AnimInstance->SetAttackEnable(true);

			m_AttackIndex = (m_AttackIndex + 1) % m_AttackMontageArray.Num();
		}
	}
	////// 0번 공격 모션이 재생이 안되고 있다면 재생을 시켜준다.
	//if (!m_AnimInstance->Montage_IsPlaying(m_AttackMontageArray[m_AttackIndex]))
	//{
	//	m_AnimInstance->Montage_SetPosition(m_AttackMontageArray[m_AttackIndex], 0.f);//ani 시간 0으로  초기화
	//	m_AnimInstance->Montage_Play(m_AttackMontageArray[m_AttackIndex]);
	//	m_AnimInstance->SetAttackEnable(true);

	//	//0 ~ 3까지 반복하도록 
	//	m_AttackIndex = (m_AttackIndex + 1) % m_AttackMontageArray.Num();
	//}
}

void AGreyStone::Skill1()
{
	// 0번 공격 모션이 재생이 안되고 있다면 재생을 시켜준다.
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

	FCollisionQueryParams	params(NAME_None, false, this); //충돌을 위한 파라미터들 

	// 근접공격으로 이 타이밍에 충돌처리를 해주도록 한다.
	TArray<FHitResult>	HitResultArray; //t어래이 타입으로 hit결과 배열을 만들어줌 , 충돌을 한 뒤 충돌의 결과값을 저장하는 구조체
	//impactpoint는 부딪힌 위치 normal은 부딪힌 방향 
	bool Sweep = GetWorld()->SweepMultiByChannel(HitResultArray, PlayerLoc, //멅티는 여러마리 싱글은 한마리 //두번째 인자는 충돌 시작점	
		PlayerLoc, FQuat::Identity, //공격 거리, 회전정보가 기본 
		ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeSphere(m_PlayerInfo.AttackDistance), //engineTrace사용xxx ,
		params);
	
	//로그 출력 가능
	//LOG(TEXT("Attack : %.5f"), 200.f);
	//LOG(TEXT("TestAttack"));
	PrintViewport(1.f, FColor::Yellow, TEXT("Attack"));


	TArray<FHitResult>	CollisionArray;
	if (Sweep)
	{
		// 1차 구충돌이 일어난 타겟과 나와의 각도를 구해준다.
		//for (int32 i = 0; i < HitResultArray.Num(); ++i)
		for (auto& result : HitResultArray)
		{
			//result.ImpactPoint = 부딪힌 위치 포인트 
			FVector	Dir = result.ImpactPoint - GetActorLocation(); //몬스터를 향하는 벡터 방향을 구함

			// 구해준 방향벡터를 단위벡터로 만든다. 그 이유는 원뿔 형태로 체크를 하기 위해서
			// 각도를 체크해줘야 하는데 단위벡터의 내적을 이용하게 된다면 더 벡터간 각도의
			// 코사인 세타 값을 구해줄 수 있다.
			// 그러므로 이러한 코사인 세타를 아크코사인을 이용하여 세타(각도)로 변경하고
			// 이를 비교하여 원뿔 안에 들어오는지를 판단할 수 있다.
			Dir.Normalize(); //단위벡터로 만듦

			Forward = GetActorForwardVector();
			Forward.Normalize();

			float Dot = FVector::DotProduct(Dir, Forward); //단위벡터의 내적을 하면 코싸인 값이 나온다

			// Acos를 이용해서 각도를 구하면 이 값은 Radian 값으로 들어오게 된다.
			// 그러므로 이를 Degree 로 변환해주고 이를 비교하는 각도로 사용한다.
			float Angle = FMath::Acos(Dot);
			Angle = FMath::RadiansToDegrees(Angle); //디그리 값으로 변환

			if (Angle <= m_PlayerInfo.AttackAngle) //절반 각도 안에 들어오는지
				CollisionArray.Add(result); //공격 범위 안에 있으므로 배열에 넣어준다.

			else
			{
				// 이게 아니라면 직선을 이용해서 상대방 충돌체를 관통하는지 판단한다.

				// 직선을 이용하여 물체가 관통하는지 확인하는 것
				//GetWorld()->LineTraceSingleByChannel()

			}
		}
	}
#if ENABLE_DRAW_DEBUG

	FColor	DrawColor = CollisionArray.Num() > 0 ? FColor::Red : FColor::Green;
	//FColor	DrawColor = Sweep  ? FColor::Red : FColor::Green;

	//DrawDebugSphere(GetWorld(), PlayerLoc, 200.f, 20, DrawColor, false, 1.f); //1초동안 디버귿용으로 충돌체 그려줌 
	DrawDebugCone(GetWorld(), PlayerLoc, GetActorForwardVector(), m_PlayerInfo.AttackDistance,
		FMath::DegreesToRadians(m_PlayerInfo.AttackAngle),
		FMath::DegreesToRadians(m_PlayerInfo.AttackAngle), 20,
		DrawColor, false, 1.f);
#endif


	for (auto& result : CollisionArray)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // 부딪히던 안부딪히던 무조건 불러오도록

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			result.ImpactPoint, result.ImpactNormal.Rotation(), param); //위치정보, 회전정보(어느각도?:부딪혔을때 방향벡터의 반대방향), 방향벡터를 회전정보로 바꾸어줌, 위에서 불러온 파라미터 정보 

		// 애셋을 로딩한다.
		Effect->LoadParticle(TEXT("ParticleSystem'/Game/AdvancedMagicFX12/particles/P_ky_hit_thunder.P_ky_hit_thunder'"));

		// Sound
		Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));


		// 데미지를 전달한다.
		FDamageEvent	DmgEvent;
		//최종 데미지
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

		//skill2 데칼을 만들어서 진행, effect에 effectdecal 을 상속받아 skill2데칼을 만든다

		FActorSpawnParameters	param;

		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//데칼 만든것을 불러옴
		m_Skill2Decal = GetWorld()->SpawnActor<AGreystoneSkill2Decal>(AGreystoneSkill2Decal::StaticClass(),
			GetActorLocation(), FRotator::ZeroRotator, param);
	}
	break;
	}
}



void AGreyStone::Skill2()
{
	// 0번 공격 모션이 재생이 안되고 있다면 재생을 시켜준다.
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
		// 여기서 메테오 발사!!!!
	}
	break;
	}
}





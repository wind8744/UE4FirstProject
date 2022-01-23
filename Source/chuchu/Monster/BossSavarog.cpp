// Fill out your copyright notice in the Description page of Project Settings.


#include "BossSavarog.h"
#include "../Player/PlayerCharacter.h"
#include "../Effect/NormalEffect.h"
#include "../Effect/HitCameraShake.h"
#include "../chuchuGameInstance.h"
#include "../UIItem/ItemBox.h"

ABossSavarog::ABossSavarog()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Meshes/Sevarog.Sevarog'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("AnimBlueprint'/Game/Monster/BPBossSevarogAnim.BPBossSevarogAnim_C'"));
	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
	
	//몽타주 (Skill)
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackAsset(TEXT("AnimMontage'/Game/Monster/AMSevarAttack.AMSevarAttack'"));
	if (AttackAsset.Succeeded())
		m_AttackMontageArray.Add(AttackAsset.Object);
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackAsset1(TEXT("AnimMontage'/Game/Monster/AMSevarAttack2.AMSevarAttack2'"));
	if (AttackAsset1.Succeeded())
		m_AttackMontageArray.Add(AttackAsset1.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackAsset2(TEXT("AnimMontage'/Game/Monster/AMSevarAttack3.AMSevarAttack3'"));
	if (AttackAsset2.Succeeded())
		m_AttackMontageArray.Add(AttackAsset2.Object);

	m_CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ColCapsuleRight"));
	m_CollisionCapsule->AttachTo(GetMesh(), TEXT("Muzzle_01"));
	//m_CollisionCapsule->SetRelativeLocationAndRotation(FVector(0.f, -17.f, 0.f), FRotator(70.f, 0.f, 0.f));
	//m_CollisionCapsule->SetRelativeScale3D(FVector(1.f, 1.5f, 1.5f));
	m_CollisionCapsule->SetCollisionProfileName(("Enemy"));

	m_MonsterInfoName = TEXT("BossSavarog");
	m_DropItemArray.Add(TEXT("Heart"));
}

void ABossSavarog::BeginPlay()
{
	Super::BeginPlay();
	m_bStart = false;
	m_AttackEnd = false;
	m_bInDistance = false;
	m_bAnim = false;
	m_fAtime = 0.f;
	m_iSkillNum = 0;
	m_BossState = EBossState::IDLE;
	SetActorRelativeScale3D(FVector(3, 3, 3)); //크기 조절
	ChangeAnimType(EMonsterAnimType::Idle);

	GetMesh()->ToggleVisibility();
	m_HPBar->ToggleVisibility();

}

void ABossSavarog::ChangeState()
{
	if (m_MonsterInfo.HP <= 0.1f)
	{
		m_BossState = EBossState::DEATH;
	}
}


void ABossSavarog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_fAtime += DeltaTime;
	if (!m_bStart)
	{
		if (m_fAtime >= 13.5f)
		{
			GetMesh()->ToggleVisibility();
			m_HPBar->ToggleVisibility();
			//AIControllerClass = ARamBossAIController::StaticClass();
			m_bStart = true;
		}
		return;
	}



	m_bInDistance = FindTarget();
	ChangeState();
	
	switch (m_BossState)
	{
	case EBossState::IDLE:
	{
		if (!m_bAnim)
		{
			m_iSkillNum = FMath::RandRange(0, 2); /////skill Num

			APlayerCharacter* Target = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
			const FMonsterInfo& MonsterInfo = GetMonsterInfo();
			FVector	MonsterLoc = GetActorLocation();
			FVector	TargetLoc = Target->GetActorLocation();
			FVector TarVec = TargetLoc - MonsterLoc;

			//타겟 방향으로 몬스터 회전
			FVector	Dir = TargetLoc - MonsterLoc;
			Dir.Normalize();
			SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));

			//PrintViewport(1.f, FColor::Red, "Idle");
			ChangeAnimType(EMonsterAnimType::Idle);
			m_bAnim = true;
		}
		m_fAtime += DeltaTime;

		if (m_bInDistance && m_fAtime > 2.f)
		{
			m_AttackEnd = false;
			m_fAtime = 0.f;
			m_bAnim = false;
			
			if(2 == m_iSkillNum) m_BossState = EBossState::RUNAWAY;
			else m_BossState = EBossState::TRACE;
		}
		break;
	}
	case EBossState::TRACE:
	{
		if (!m_bAnim)
		{
			//PrintViewport(1.f, FColor::Red, "trace");
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
			//PrintViewport(1.f, FColor::Red, "Runaway");
			ChangeAnimType(EMonsterAnimType::Skill);
			m_bAnim = true;
		}
		RunAway(DeltaTime);

		m_fAtime += DeltaTime;
		if (m_fAtime > 2.5f|| m_fTarDis >= 2600.f)
		{
			m_fAtime = 0.f;
			m_bAnim = false;
			m_BossState = EBossState::ATTACK;
		}
		break;
	}

	case EBossState::DEATH:
		ChangeAnimType(EMonsterAnimType::Death);
		break;
	}
	
}
#include "../Effect/SevarogProjectileEffect.h"
void ABossSavarog::NormalAttack()
{
	switch (m_iSkillNum)
	{
	case 0:
	{
		FVector EffecLoc = GetActorLocation();
		EffecLoc.Z -= 150;
		// Particle Effect
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ANormalEffect* Effect2 = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(), EffecLoc, GetActorRotation(), param);
		Effect2->LoadParticleAsync(TEXT("SevarogAttack"));

		break;
	}
	case 1:
	{
		//APlayerCharacter* Target = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		FVector	SocketLoc = GetMesh()->GetSocketLocation(TEXT("Weapon_01")); //소켓 위치를 얻어울수있다.
		SocketLoc.Z = 30.f;
		// Particle Effect
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ANormalEffect* Effect2 = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(), SocketLoc, GetActorRotation(), param);
		Effect2->LoadParticleAsync(TEXT("SevarogAttack4"));

		break;
	}
	case 2:
	{
		FVector EffecLoc = GetActorLocation();
		EffecLoc.Z -= 120;

		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ASevarogProjectileEffect* Bullet = GetWorld()->SpawnActor<ASevarogProjectileEffect>(ASevarogProjectileEffect::StaticClass(), EffecLoc, GetActorRotation(),param);
		Bullet->SetAttack(m_MonsterInfo.Attack);
		Bullet->SetOwner(this);

		break;
	}
	default:
		break;
	}

	if (2 != m_iSkillNum)
	{
		//카메라 쉐이트
		GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(UHitCameraShake::StaticClass());

		// 충돌체
		FHitResult result2;
		FVector MonLoc = GetActorLocation();
		MonLoc.Z -= 50;
		FVector	Forward = GetActorForwardVector();

		FCollisionQueryParams	params(NAME_None, false, this); //충돌을 위한 파라미터들 

		// 근접공격으로 이 타이밍에 충돌처리를 해주도록 한다.
		TArray<FHitResult>	HitResultArray;
		FVector AttackBox;
		AttackBox.X = 300.f;
		AttackBox.Y = 300.f;
		AttackBox.Z = 150.f;

		bool Sweep = GetWorld()->SweepMultiByChannel(HitResultArray, MonLoc, //멅티는 여러마리 싱글은 한마리
			MonLoc, FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeBox(AttackBox),
			params);


		//#if ENABLE_DRAW_DEBUG
		//	PrintViewport(1.f, FColor::Yellow, TEXT("HHHHit"));
		//	FColor	DrawColor = HitResultArray.Num() > 0 ? FColor::Red : FColor::Green;
		//	DrawDebugBox(GetWorld(), MonLoc, AttackBox, DrawColor, false, 1.f);
		//#endif

		for (auto& result : HitResultArray)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // 부딪히던 안부딪히던 무조건 불러오도록

			ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
				result.ImpactPoint, result.ImpactNormal.Rotation(), param); //위치정보, 회전정보(어느각도?:부딪혔을때 방향벡터의 반대방향), 방향벡터를 회전정보로 바꾸어줌, 위에서 불러온 파라미터 정보 
			Effect->LoadParticle(TEXT("ParticleSystem'/Game/FantasyVFXCollection/3_ParticleSystem/Hit/Basic_Hit/P_Hit_04.P_Hit_04'"));
			// Sound
			//Effect->LoadSoundAsync(TEXT("HitNormal"));
			//Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));

			FDamageEvent	DmgEvent;
			float Damage = result.GetActor()->TakeDamage(20.f, DmgEvent, GetController(), this);
		}
	}
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

	//LOG(TEXT("dis : %.5f"), m_fTarDis);

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

		MonsterLoc.X += TarVec.X * _dt * 500.f; // MonsterInfo.MoveSpeed;
		MonsterLoc.Y += TarVec.Y * _dt * 500.f;// MonsterInfo.MoveSpeed;

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
	case 2:
	{
		if (!m_bAnim)
		{
			APlayerCharacter* Target = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
			const FMonsterInfo& MonsterInfo = GetMonsterInfo();
			FVector	MonsterLoc = GetActorLocation();
			FVector	TargetLoc = Target->GetActorLocation();
			FVector TarVec = TargetLoc - MonsterLoc;

			//타겟 방향으로 몬스터 회전
			FVector	Dir = TargetLoc - MonsterLoc;
			Dir.Normalize();
			SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));

			//PrintViewport(1.f, FColor::Red, "attack2222");

			if (!m_AnimInstance->Montage_IsPlaying(m_AttackMontageArray[m_iSkillNum]))
			{
				m_AnimInstance->Montage_SetPosition(m_AttackMontageArray[m_iSkillNum], 0.f);
				m_AnimInstance->Montage_Play(m_AttackMontageArray[m_iSkillNum]);
			}
			m_bAnim = true;
		}

		if (m_AttackEnd)
		{
			m_fAtime = 0.f;
			m_bAnim = false;
			m_BossState = EBossState::IDLE;
		}
		break;
	}
	default:
	{
		if (!m_bAnim)
		{
			APlayerCharacter* Target = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
			const FMonsterInfo& MonsterInfo = GetMonsterInfo();
			FVector	MonsterLoc = GetActorLocation();
			FVector	TargetLoc = Target->GetActorLocation();
			FVector TarVec = TargetLoc - MonsterLoc;

			//타겟 방향으로 몬스터 회전
			FVector	Dir = TargetLoc - MonsterLoc;
			Dir.Normalize();
			SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));

			//PrintViewport(1.f, FColor::Red, "attack");
			//ChangeAnimType(EMonsterAnimType::Attack);
			if (!m_AnimInstance->Montage_IsPlaying(m_AttackMontageArray[m_iSkillNum]))
			{
				m_AnimInstance->Montage_SetPosition(m_AttackMontageArray[m_iSkillNum], 0.f);
				m_AnimInstance->Montage_Play(m_AttackMontageArray[m_iSkillNum]);
			}
			m_bAnim = true;
		}

		if (m_AttackEnd)
		{
			m_fAtime = 0.f;
			m_bAnim = false;
			
			if (m_fTarDis <= 350.f) //범위내 있으면 계속 공격
				m_BossState = EBossState::ATTACK; //m_BossState = EBossState::RUNAWAY;
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

	MonsterLoc.X += -TarVec.X * _dt * 700.f;
	MonsterLoc.Y += -TarVec.Y * _dt * 700.f;

	SetActorLocation(MonsterLoc);

	//타겟 방향으로 몬스터 회전
	FVector	Dir = TargetLoc - MonsterLoc;
	Dir.Normalize();
	SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
}

void ABossSavarog::Death()
{
	//int32 itemindex = 0;
	//UchuchuGameInstance* gameinst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance()); // 게임 인스턴스는 레벨이 몇개이던 하나만 만들어짐
	//if (gameinst)
	//{
	//	const FUIItemDataInfo* iteminfo = gameinst->FindUIItemInfo(m_DropItemArray[itemindex]);
	//	if (iteminfo)
	//	{
	//		FActorSpawnParameters itemparam;
	//		itemparam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//		FBox dropbox(GetMesh()->GetComponentLocation() - FVector(50.f, 50.f, -50.f), GetMesh()->GetComponentLocation() + FVector(50.f, 50.f, 100.f)); //Min, Max
	//		AItemBox* Itembox = GetWorld()->SpawnActor<AItemBox>(AItemBox::StaticClass(), FMath::RandPointInBox(dropbox), GetMesh()->GetComponentRotation(), itemparam);
	//		if (Itembox)
	//		{
	//			Itembox->SetMesh(iteminfo->m_PickMesh);
	//			Itembox->SetItemName(iteminfo->m_ItemName);
	//			PrintViewport(10.f, FColor::Blue, FString::Printf(TEXT("Dropitem : %s"), *iteminfo->m_ItemName));
	//		}
	//	}
	//}

	Destroy();
}
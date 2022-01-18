// Fill out your copyright notice in the Description page of Project Settings.


#include "BossRampage.h"
#include "RamBossAIController.h"
#include "BuffAIController.h"
#include "../Effect/NormalEffect.h"

// Sets default values
ABossRampage::ABossRampage()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BossMeshAsset(TEXT("SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Meshes/Rampage.Rampage'"));
	if (BossMeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(BossMeshAsset.Object);
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> BossAnimAsset(TEXT("AnimBlueprint'/Game/Monster/BPRamBossAnim.BPRamBossAnim_C'"));
	if (BossAnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(BossAnimAsset.Class);
	
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatAsset(TEXT("MaterialInstanceConstant'/Game/ParagonRampage/Characters/Heroes/Rampage/Materials/M_RampageSkin_MASTER_Inst.M_RampageSkin_MASTER_Inst'"));
	if (MatAsset.Succeeded())
		m_RedMat = MatAsset.Object;

	
	// 오른쪽 팔 충돌체
	
	m_CollisionCapsuleRight = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ColCapsuleRight"));
	m_CollisionCapsuleRight->AttachTo(GetMesh(),TEXT("Muzzle_01"));
	m_CollisionCapsuleRight->SetRelativeLocationAndRotation(FVector(0.f, -17.f, 0.f), FRotator(70.f,0.f,0.f));
	m_CollisionCapsuleRight->SetRelativeScale3D(FVector(1.f, 1.5f, 1.5f));
	m_CollisionCapsuleRight->SetCollisionProfileName(("Enemy"));
	//m_CollisionCapsuleRight->OnComponentBeginOverlap.AddDynamic(this, &AGhostLady::OnComponentBeginOverlapWeapon);
	
	// 왼쪽 팔 충돌체
	m_CollisionCapsuleLeft = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ColCapsuleLeft"));
	m_CollisionCapsuleLeft->AttachTo(GetMesh(), TEXT("Muzzle_02"));
	m_CollisionCapsuleLeft->SetRelativeLocationAndRotation(FVector(0.f, -17.f, 0.f), FRotator(70.f, 0.f, 0.f));
	m_CollisionCapsuleLeft->SetRelativeScale3D(FVector(1.f, 1.5f, 1.5f));
	m_CollisionCapsuleLeft->SetCollisionProfileName(("Enemy"));
	//m_CollisionCapsuleLeft->OnComponentBeginOverlap.AddDynamic(this, &AGhostLady::OnComponentBeginOverlapWeapon);
	

	// 그냥 부모 MOnsterAI컨트롤러가 기본이 되어이쓰으모로 바꺼준다
	AIControllerClass = ARamBossAIController::StaticClass();

	m_MonsterInfoName = TEXT("BossRampage");
	m_SkillNum = 0;
	
	m_Atime = 0.f;
	m_EnableAI = false;


	// 드랍 아이템
	m_DropItemArray.Add(TEXT("MpPotion"));
}

// Called when the game starts or when spawned
void ABossRampage::BeginPlay()
{
	Super::BeginPlay();

	//AIControllerClass = nullptr;
	GetMesh()->ToggleVisibility();
	m_HPBar->ToggleVisibility();
	SetActorRelativeScale3D(FVector(3, 3, 3)); //크기 조절
}

// Called every frame
void ABossRampage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	m_Atime += DeltaTime;
	if (!m_EnableAI)
	{
		if (m_Atime >= 30.f)
		{
			GetMesh()->ToggleVisibility();
			m_HPBar->ToggleVisibility();
			//AIControllerClass = ARamBossAIController::StaticClass();
			m_EnableAI = true;
		}
	}

}

void ABossRampage::NormalAttack()
{
	// 충돌체
	FHitResult result2;
	FVector	Forward = GetActorForwardVector();
	FCollisionQueryParams	params(NAME_None, false, this); //충돌을 위한 파라미터들 

	// 근접공격으로 이 타이밍에 충돌처리를 해주도록 한다.
	TArray<FHitResult>	HitResultArray;

	FVector AttackBox;
	AttackBox.X = 400.f;
	AttackBox.Y = 400.f;
	AttackBox.Z = 300.f;

	bool Sweep = GetWorld()->SweepMultiByChannel(HitResultArray, GetActorLocation(),
		GetActorLocation(), FQuat::Identity,ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeBox(AttackBox),params);


#if ENABLE_DRAW_DEBUG
	PrintViewport(1.f, FColor::Yellow, TEXT("Attack"));
	FColor	DrawColor = HitResultArray.Num() > 0 ? FColor::Red : FColor::Green;
	DrawDebugBox(GetWorld(), GetActorLocation(), AttackBox, DrawColor, false, 1.f);
#endif

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // 부딪히던 안부딪히던 무조건 불러오도록
	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(), GetActorLocation(),GetActorRotation(), param);

	// 애셋을 로딩한다.
	Effect->LoadParticle(TEXT("ParticleSystem'/Game/ParagonRampage/FX/Particles/Abilities/RipNToss/FX/P_RipNToss_HandImpact.P_RipNToss_HandImpact'"));

	for (auto& result : HitResultArray)
	{
		FActorSpawnParameters	param1;
		param1.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // 부딪히던 안부딪히던 무조건 불러오도록

		ANormalEffect* Effect1 = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			result.ImpactPoint, result.ImpactNormal.Rotation(), param1); //위치정보, 회전정보(어느각도?:부딪혔을때 방향벡터의 반대방향), 방향벡터를 회전정보로 바꾸어줌, 위에서 불러온 파라미터 정보 

		// 애셋을 로딩한다.
		Effect1->LoadParticle(TEXT("ParticleSystem'/Game/ParagonRampage/FX/Particles/Abilities/Primary/FX/P_Rampage_Melee_Impact.P_Rampage_Melee_Impact'"));

		// Sound
		//Effect->LoadSoundAsync(TEXT("HitNormal"));
		//Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));

		// 데미지를 전달한다.
		FDamageEvent	DmgEvent;
		//최종 데미지
		float Damage = result.GetActor()->TakeDamage(m_MonsterInfo.Attack, DmgEvent, GetController(), this);

	}
}

//데미지를 줄 수 있는 함수 재정의
float ABossRampage::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//Make REd Monster


	for (int i = 0; i < 3; i++)
	{
		m_CurMatArray.Add(GetMesh()->GetMaterial(i));
		GetMesh()->SetMaterial(i,m_RedMat);
	}

	GetWorldTimerManager().SetTimer(m_OriMatTimer, this, &ABossRampage::MakeOriMaterial, 0.3f, false); 			//2초후 원래대로
	
	return Damage;
}

void ABossRampage::MakeOriMaterial()
{
	for (int i = 0; i < 3; i++)
	{
		GetMesh()->SetMaterial(i, m_CurMatArray[i]);
	}

}
void ABossRampage::Death()
{
	//m_SpawnPoint->Death();
	Destroy();
}
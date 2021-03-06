// Fill out your copyright notice in the Description page of Project Settings.

#include "BuffRed.h"
#include "../Effect/NormalEffect.h"

// Sets default values
ABuffRed::ABuffRed()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Buff/Buff_Red/Meshes/Buff_Red.Buff_Red'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	// 블루프린트 사용시 _C 꼭 붙여줄것
	static ConstructorHelpers::FClassFinder<UAnimInstance>	 AnimAsset(TEXT("AnimBlueprint'/Game/Monster/BPBuffRedAnim.BPBuffRedAnim_C'"));

	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	m_MonsterInfoName = TEXT("BuffRed");

}

// Called when the game starts or when spawned
void ABuffRed::BeginPlay()
{
	Super::BeginPlay();
	
	ChangeAnimType(EMonsterAnimType::Spawn);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector EffLoc = GetActorLocation();
	EffLoc.Z = EffLoc.Z - 100.f;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		EffLoc, GetActorRotation(), param);

	// 애셋을 로딩한다.
	Effect->LoadParticle(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/Buffs/Buff_Red/FX/P_Buff_Red_SpawnFX.P_Buff_Red_SpawnFX'"));

}

// Called every frame
void ABuffRed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuffRed::NormalAttack()
{
	
	FVector	MinionLoc = GetActorLocation();
	FVector	Forward = GetActorForwardVector();

	FCollisionQueryParams	params(NAME_None, false, this);

	// 근접공격으로 이 타이밍에 충돌처리를 해주도록 한다.
	FHitResult	result;
	bool Sweep = GetWorld()->SweepSingleByChannel(result, MinionLoc, //한명만 조짐

		MinionLoc + Forward * m_MonsterInfo.AttackDistance, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel7,
		FCollisionShape::MakeSphere(30.f),
		params);

#if ENABLE_DRAW_DEBUG

	FColor	DrawColor = Sweep ? FColor::Red : FColor::Green;

	FVector Center = MinionLoc + Forward * m_MonsterInfo.AttackDistance / 2.f; //절반위치

	DrawDebugCapsule(GetWorld(), Center, m_MonsterInfo.AttackDistance / 2.f,
		30.f, FRotationMatrix::MakeFromZ(Forward).ToQuat(), //전방방향 바라보도록 회전
		DrawColor, false, 0.5f);

#endif

	if (Sweep)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			result.ImpactPoint, result.ImpactNormal.Rotation(), param);

		// 애셋을 로딩한다.
		Effect->LoadParticle(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/Buffs/Buff_Red/FX/P_Buff_Red_Melee_Impact.P_Buff_Red_Melee_Impact'"));

		// Sound
		//Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));


		// 데미지를 전달한다.
		FDamageEvent	DmgEvent;
		float Damage = result.GetActor()->TakeDamage(m_MonsterInfo.Attack, DmgEvent, GetController(), this);
	}
	
}

void ABuffRed::SpawnEnd()
{
	ChangeAnimType(EMonsterAnimType::Idle);
}
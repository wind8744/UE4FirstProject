// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionGunnerBullet.h"
#include "NormalEffect.h"
#include "../Monster/Monster.h"

// Sets default values
AMinionGunnerBullet::AMinionGunnerBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/AdvancedMagicFX13/Particles/P_ky_stabShot.P_ky_stabShot'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/Buffs/Buff_Blue/FX/P_Spit_Attack_Projectile.P_Spit_Attack_Projectile'"));
	
	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	m_Movement->InitialSpeed = 2000.f;
}

// Called when the game starts or when spawned
void AMinionGunnerBullet::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMinionGunnerBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMinionGunnerBullet::StopEvent(const FHitResult& result)
{
	Super::StopEvent(result);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		result.ImpactPoint, result.Normal.Rotation(), param);

	// 애셋을 로딩한다.
	//Effect->LoadParticle(TEXT("ParticleSystem'/Game/AdvancedMagicFX13/Particles/P_ky_stab.P_ky_stab'"));

	// Sound
	//Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));

	m_Particle->DestroyComponent();

	if (IsValid(m_Owner)) //유효한지 아닌지체크
	{
		FDamageEvent	DmgEvent;
		// 원거리공격이므로 여기서 처리해주어얗 한다
		result.GetActor()->TakeDamage(m_Attack, DmgEvent,
			m_Owner->GetController(), m_Owner);
	}
}

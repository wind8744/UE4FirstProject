// Fill out your copyright notice in the Description page of Project Settings.


#include "SevarogProjectileEffect.h"
#include "NormalEffect.h"
#include "../Monster/Monster.h"

// Sets default values
ASevarogProjectileEffect::ASevarogProjectileEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/FantasyVFXCollection/3_ParticleSystem/Projectile/Projectile/Projectile_02/P_Projectile_02_04.P_Projectile_02_04'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	m_Movement->InitialSpeed = 3000.f;
	m_fDestroyTime = 0.f;
}

// Called when the game starts or when spawned
void ASevarogProjectileEffect::BeginPlay()
{
	Super::BeginPlay();
	SetActorRelativeScale3D(FVector(2, 2, 2)); //크기 조절
}

// Called every frame
void ASevarogProjectileEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_fDestroyTime += DeltaTime;
	if (m_fDestroyTime > 7.f)
	{
		//m_Particle->DestroyComponent();
		Destroy();
	}
}

void ASevarogProjectileEffect::StopEvent(const FHitResult& result)
{
	Super::StopEvent(result);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),result.ImpactPoint, result.Normal.Rotation(), param);

	// 애셋을 로딩한다.
	Effect->LoadParticle(TEXT("ParticleSystem'/Game/ParagonSevarog/FX/Particles/Abilities/Ultimate/FX/P_UltDamageTrails.P_UltDamageTrails'"));
	//Effect->LoadSound(TEXT("ParticleSystem'/Game/ParagonSevarog/FX/Particles/Abilities/SoulSiphon/FX/P_SiphonImpact.P_SiphonImpact'"));

	m_Particle->DestroyComponent();

	// 원거리공격이므로 여기서 처리해주어얗 한다
	if (IsValid(m_Owner))
	{
		FDamageEvent	DmgEvent;
		result.GetActor()->TakeDamage(m_Attack, DmgEvent,m_Owner->GetController(), m_Owner);
	}
}


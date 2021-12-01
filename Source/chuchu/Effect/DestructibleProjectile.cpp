// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleProjectile.h"

// Sets default values
ADestructibleProjectile::ADestructibleProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Body = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	m_Mesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Mesh"));
	m_Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	SetRootComponent(m_Body);

	m_Mesh->SetupAttachment(m_Body);

	m_Movement->SetUpdatedComponent(m_Body);

	m_Movement->InitialSpeed = 1000.f;

	m_DestroyEnable = false;
	m_DestroyTime = 0.f;
	m_DestroyTimeMax = 5.f;

	m_Distance = 2000.f; //기본 날아갈 수 있는 거리 20m

	m_Mesh->SetReceivesDecals(false);
}

// Called when the game starts or when spawned
void ADestructibleProjectile::BeginPlay()
{
	Super::BeginPlay();

	//멈추면 삭제되도록 함수 주소 걸어둠
	m_Movement->OnProjectileStop.AddDynamic(this, &ADestructibleProjectile::ProjectileStop);
	
}

// Called every frame
void ADestructibleProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_Distance -= m_Movement->Velocity.Size() * DeltaTime;

	//PrintViewport(1.f, FColor::Red, m_Movement->Velocity.ToString());
	//PrintViewport(1.f, FColor::Blue, FString::Printf(TEXT("Dist : %.5f"), m_Distance));

	if (m_Distance <= 0.f)
		Destroy();

	if (m_DestroyEnable)
	{
		m_DestroyTime += DeltaTime; //atime
		
		if (m_DestroyTimeMax <= m_DestroyTime)
		{

			Destroy();
		}
			
	}
}


void ADestructibleProjectile::ProjectileStop(const FHitResult& result)
{
	//부딪혔을때 
	//PrintViewport(1.f, FColor::Red, TEXT("projectile stop"));
	StopEvent(result); //그래이스톤스킬1클래스가 호출하는것이므로 가상함수여서 그래이스톤스킬클래스 StopEvent 함수가 호출
}

void ADestructibleProjectile::StopEvent(const FHitResult& result)
{
	PrintViewport(1.f, FColor::Blue, TEXT("projectile stop"));
	//메쉬에 데미지를 가해서 부술 수 있도록 
	//m_Mesh->ApplyRadiusDamage(1000.f, result.Normal, 1000.f, 1000.f, true);
	m_Mesh->ApplyDamage(10.f, result.ImpactPoint, result.ImpactNormal, 10.f);

	m_DestroyEnable = true;
}

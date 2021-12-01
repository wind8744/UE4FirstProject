// Fill out your copyright notice in the Description page of Project Settings.


#include "Meteor.h"

// Sets default values
AMeteor::AMeteor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Body = CreateDefaultSubobject<USphereComponent>(TEXT("Body"));
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	m_Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	SetRootComponent(m_Body);

	m_Mesh->SetupAttachment(m_Body);
	m_Particle->SetupAttachment(m_Mesh);
	m_Body->SetSphereRadius(130.f);

	m_Mesh->SetRelativeLocation(FVector(0.f, 0.f, -20.f));

	m_Particle->SetRelativeLocation(FVector(0.f, 70.f, 30.f));
	m_Particle->SetRelativeScale3D(FVector(2.f, 1.f, 2.f));

	m_Mesh->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));

	m_Movement->Velocity = FVector(0.f, 0.f, -1.f);
	m_Movement->InitialSpeed = 1000.f;

	m_Movement->SetUpdatedComponent(m_Body); 

	static ConstructorHelpers::FObjectFinder<UStaticMesh>	MeshAsset(TEXT("StaticMesh'/Game/Light_Foliage/Meshes/SM_Rock_01.SM_Rock_01'"));

	if (MeshAsset.Succeeded())
		m_Mesh->SetStaticMesh(MeshAsset.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/AdvancedMagicFX12/particles/P_ky_shot_fire.P_ky_shot_fire'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_Body->SetCollisionProfileName(TEXT("EnemyAttack"));
}

// Called when the game starts or when spawned
void AMeteor::BeginPlay()
{
	Super::BeginPlay();

	m_Movement->OnProjectileStop.AddDynamic(this, &AMeteor::ProjectileStop);
}

// Called every frame
void AMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeteor::ProjectileStop(const FHitResult& result)
{
	//StopEvent(result);
	PrintViewport(1.f, FColor::Blue, TEXT("Meteor Hit"));
}


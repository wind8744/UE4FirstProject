// Fill out your copyright notice in the Description page of Project Settings.


#include "GreyStoneSkill1.h"
#include "NormalEffect.h"
#include "GreyStoneSkill1Decal.h"

// Sets default values
AGreyStoneSkill1::AGreyStoneSkill1()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	m_Trail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));

	m_Particle->SetupAttachment(m_Body); //�޽� �ٵ� ����
	m_Trail->SetupAttachment(m_Body);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	Asset(TEXT("ParticleSystem'/Game/AdvancedMagicFX13/Particles/P_ky_cutter2.P_ky_cutter2'"));

	if (Asset.Succeeded())
		m_Particle->SetTemplate(Asset.Object);

	//Trail
	static ConstructorHelpers::FObjectFinder<UParticleSystem>	Asset1(TEXT("ParticleSystem'/Game/Particle/PSPlayerTrail.PSPlayerTrail'"));

	if (Asset1.Succeeded())
		m_Trail->SetTemplate(Asset1.Object);
}

// Called when the game starts or when spawned
void AGreyStoneSkill1::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGreyStoneSkill1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �������°� �ƴҶ��� 
	if (!m_DestroyEnable)
		m_Mesh->AddRelativeRotation(FRotator(0.f, 1080.f * DeltaTime, 0.f)); //ȸ��
}

void AGreyStoneSkill1::StopEvent(const FHitResult& result)
{
	Super::StopEvent(result);//�����Լ� //GreyStoneSkill1Ŭ������ �ڱ� �θ� ȣ��......DestructibleProjectile��stopEvent�Լ� 
	
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		result.ImpactPoint, result.ImpactNormal.Rotation(), param);

	// �ּ��� �ε��Ѵ�.
	//Effect->LoadParticle(TEXT("ParticleSystem'/Game/AdvancedMagicFX13/Particles/P_ky_explosion3.P_ky_explosion3'"));
	Effect->LoadParticleAsync(TEXT("HitFire"));

	// Sound
	//Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));
	Effect->LoadSoundAsync(TEXT("HitFire"));

	m_Particle->DestroyComponent(); //�ε����� �� ���ֱ�

	// ���� ��ġ���� �Ʒ��� ���� üũ�� �Ѵ�.
	FCollisionQueryParams	params(NAME_None, false, this);

	FHitResult	LineResult;
	bool Collision = GetWorld()->LineTraceSingleByChannel(LineResult, GetActorLocation(), //���� ��ġ
		GetActorLocation() + GetActorUpVector() * -300.f, //������ġ�� �ݴ�������� 3m������ ó��
		ECollisionChannel::ECC_GameTraceChannel5, //�ݸ����� DefaultEngine.ini���� RayLandScapeã�Ƽ� ä�� �����ֱ�
		params);

	//�浹�� �Ǿ����� ��Į �̹��� ����
	if (Collision)
	{
		FActorSpawnParameters	param1;
		param1.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AGreyStoneSkill1Decal* Decal = GetWorld()->SpawnActor<AGreyStoneSkill1Decal>(AGreyStoneSkill1Decal::StaticClass(),
			LineResult.ImpactPoint, FRotator::ZeroRotator, param1);
	}
}


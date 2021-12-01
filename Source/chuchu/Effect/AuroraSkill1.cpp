// Fill out your copyright notice in the Description page of Project Settings.


#include "AuroraSkill1.h"
#include "NormalEffect.h"


// Sets default values
AAuroraSkill1::AAuroraSkill1()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	m_Trail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));

	m_Particle->SetupAttachment(m_Body); //�޽� �ٵ� ����
	m_Trail->SetupAttachment(m_Body);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	Asset(TEXT("ParticleSystem'/Game/AdvancedMagicFX13/Particles/P_ky_cutter3.P_ky_cutter3'"));

	if (Asset.Succeeded())
		m_Particle->SetTemplate(Asset.Object);

	//test -> ��ƼŬ�� �ٿ�����
// �ּ��� �ε��Ѵ�.
//FActorSpawnParameters	param;
//param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
// effect 1
//ANormalEffect* Effect1 = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
//	GetActorLocation(), GetActorRotation(), param);
//Effect1->LoadParticle(TEXT("ParticleSystem'/Game/ParagonAurora/FX/Particles/Abilities/Dash/FX/P_Aurora_e_Wall_Top_Destroy.P_Aurora_e_Wall_Top_Destroy'"));
//test


	//Trail
	//static ConstructorHelpers::FObjectFinder<UParticleSystem>	Asset1(TEXT("ParticleSystem'/Game/Particle/PSPlayerTrail.PSPlayerTrail'"));

	//if (Asset1.Succeeded())
	//	m_Trail->SetTemplate(Asset1.Object);
}

// Called when the game starts or when spawned
void AAuroraSkill1::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAuroraSkill1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �������°� �ƴҶ��� 
	if (!m_DestroyEnable)
		m_Mesh->AddRelativeRotation(FRotator(0.f, 1080.f * DeltaTime, 0.f)); //ȸ��
}

void AAuroraSkill1::StopEvent(const FHitResult& result)
{
	Super::StopEvent(result);//�����Լ� //GreyStoneSkill1Ŭ������ �ڱ� �θ� ȣ��......DestructibleProjectile��stopEvent�Լ� 

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// �ּ��� �ε��Ѵ�.

	// effect 1
	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		result.ImpactPoint, result.ImpactNormal.Rotation(), param);
	Effect->LoadParticleAsync(TEXT("AuroraSkill1A"));
	//Effect->LoadParticle(TEXT("ParticleSystem'/Game/ParagonAurora/FX/Particles/Abilities/Ultimate/FX/P_Aurora_Ultimate_Explode_Cheap.P_Aurora_Ultimate_Explode_Cheap'"));
	
	// effect 2
	ANormalEffect* Effect1 = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		result.ImpactPoint, result.ImpactNormal.Rotation(), param);
	Effect1->LoadParticleAsync(TEXT("AuroraSkill1B"));
	//Effect1->LoadParticle(TEXT("ParticleSystem'/Game/ParagonAurora/FX/Particles/Abilities/Freeze/FX/P_Aurora_Freeze_Segment.P_Aurora_Freeze_Segment'"));

	//���̾ư���
	/*
	ANormalEffect* Effect2 = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		result.ImpactPoint, result.ImpactNormal.Rotation(), param);

	// �ּ��� �ε��Ѵ�.
	Effect2->LoadNiagara(TEXT("NiagaraSystem'/Game/AdvancedMagicFX13/Particles/Niagara/NS_ky_storm.NS_ky_storm'"), result.ImpactPoint);
	*/

	//UNiagaraSystem* ns = LoadObject<UNiagaraSystem>(GetWorld(), (TEXT("NiagaraSystem'/Game/AdvancedMagicFX13/Particles/Niagara/NS_ky_storm.NS_ky_storm'")));
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ns, result.ImpactPoint);
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_ns, result.ImpactPoint);

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

		//AGreyStoneSkill1Decal* Decal = GetWorld()->SpawnActor<AGreyStoneSkill1Decal>(AGreyStoneSkill1Decal::StaticClass(),
			//LineResult.ImpactPoint, FRotator::ZeroRotator, param1);
	}
}




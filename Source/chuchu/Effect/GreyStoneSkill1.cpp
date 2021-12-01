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

	m_Particle->SetupAttachment(m_Body); //메쉬 바디에 붙임
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

	// 죽은상태가 아닐때만 
	if (!m_DestroyEnable)
		m_Mesh->AddRelativeRotation(FRotator(0.f, 1080.f * DeltaTime, 0.f)); //회전
}

void AGreyStoneSkill1::StopEvent(const FHitResult& result)
{
	Super::StopEvent(result);//가상함수 //GreyStoneSkill1클래스가 자기 부모 호출......DestructibleProjectile의stopEvent함수 
	
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		result.ImpactPoint, result.ImpactNormal.Rotation(), param);

	// 애셋을 로딩한다.
	//Effect->LoadParticle(TEXT("ParticleSystem'/Game/AdvancedMagicFX13/Particles/P_ky_explosion3.P_ky_explosion3'"));
	Effect->LoadParticleAsync(TEXT("HitFire"));

	// Sound
	//Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));
	Effect->LoadSoundAsync(TEXT("HitFire"));

	m_Particle->DestroyComponent(); //부딪혔을 때 없애기

	// 현재 위치에서 아래로 레이 체크를 한다.
	FCollisionQueryParams	params(NAME_None, false, this);

	FHitResult	LineResult;
	bool Collision = GetWorld()->LineTraceSingleByChannel(LineResult, GetActorLocation(), //액터 위치
		GetActorLocation() + GetActorUpVector() * -300.f, //액터위치의 반대방향으로 3m까지만 처리
		ECollisionChannel::ECC_GameTraceChannel5, //콜리젼은 DefaultEngine.ini가면 RayLandScape찾아서 채널 적어주기
		params);

	//충돌이 되었으면 데칼 이미지 생성
	if (Collision)
	{
		FActorSpawnParameters	param1;
		param1.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AGreyStoneSkill1Decal* Decal = GetWorld()->SpawnActor<AGreyStoneSkill1Decal>(AGreyStoneSkill1Decal::StaticClass(),
			LineResult.ImpactPoint, FRotator::ZeroRotator, param1);
	}
}


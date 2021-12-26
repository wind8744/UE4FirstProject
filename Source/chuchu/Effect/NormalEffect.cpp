// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalEffect.h"
#include "../AssetManager/AssetPathMain.h"

// Sets default values
ANormalEffect::ANormalEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));

	SetRootComponent(m_ParticleSystem);

	m_AssetLoop = false;
}

// Called when the game starts or when spawned
void ANormalEffect::BeginPlay()
{
	Super::BeginPlay();


	// OnSystemFinished : 이 파티클시스템컴포넌트가 가지고 있는 파티클시스템이 재생이
	// 모두 종료가 되었을 경우 호출될 함수를 지정해둔다.
	// 종료시 지정된 함수가 호출된다.
	if (!m_AssetLoop)
		m_ParticleSystem->OnSystemFinished.AddDynamic(this, &ANormalEffect::ParticleFinish); //종료일때 함수를 부르며 지워줌


}

// Called every frame
void ANormalEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ANormalEffect::LoadNiagara(const FString& Path)
{
	//나이아가라
	UNiagaraSystem* ns = LoadObject<UNiagaraSystem>(GetWorld(),*Path);

	if (ns)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ns, GetActorLocation(),GetActorRotation());
}

void ANormalEffect::LoadParticle(const FString& Path)
{
	UParticleSystem* Particle = LoadObject<UParticleSystem>(GetWorld(), *Path);

	if (Particle)
		m_ParticleSystem->SetTemplate(Particle);
	
		
}
void ANormalEffect::LoadSound(const FString& Path)
{
	USoundBase* Sound = LoadObject<USoundBase>(nullptr, *Path);

	if (Sound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation());
}

void ANormalEffect::ParticleFinish(UParticleSystemComponent* Particle)
{
	Destroy();
}


void ANormalEffect::LoadSoundAsync(const FString& Name)
{
	UAssetPathMain* AssetPath = UAssetPathMain::StaticClass()->GetDefaultObject<UAssetPathMain>();

	const FSoftObjectPath* Path = AssetPath->FindSoundPath(Name);

	if (!Path)
		return;

	m_AsyncSoundPath = *Path;

	FStreamableManager& streamMgr = UAssetManager::GetStreamableManager();

	m_AsyncSoundLoadHandle = streamMgr.RequestAsyncLoad(m_AsyncSoundPath,
		FStreamableDelegate::CreateUObject(this, &ANormalEffect::LoadSoundAsyncComplete));
}

void ANormalEffect::LoadSoundAsyncComplete()
{
	m_AsyncSoundLoadHandle->ReleaseHandle();

	TAssetPtr<USoundBase>	NewSound(m_AsyncSoundPath);

	if (NewSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), NewSound.Get(), GetActorLocation());
}

void ANormalEffect::LoadParticleAsync(const FString& Name)
{
	UAssetPathMain* AssetPath = UAssetPathMain::StaticClass()->GetDefaultObject<UAssetPathMain>();

	const FSoftObjectPath* Path = AssetPath->FindParticlePath(Name);

	if (!Path)
		return;

	//패스를 가지고 와서 일단 저장
	m_AsyncParticlePath = *Path;

	// UAssetManaget ->언리얼에서 제공되는 매니저 , 엔진이 가지고있는 에셋매니저를 갖고옴(싱글톤으로 되어있다.)
	// 그녀석의 객체를 리턴해줌, 비동기를 시도
	FStreamableManager& streamMgr = UAssetManager::GetStreamableManager();

	//인자를 넣어준다.... 호출될 함수를 지정해준다.(델리게이트처럼) 델리게이트름 만들어서 지정
	m_AsyncParticleLoadHandle = streamMgr.RequestAsyncLoad(m_AsyncParticlePath,
		FStreamableDelegate::CreateUObject(this, &ANormalEffect::LoadParticleAsyncComplete));
}


void ANormalEffect::LoadParticleAsyncComplete()
{
	//로딩이 끝났을 대 핸들을 릴리즈 해줌 아까 받아놨던것 (m_AsyncParticleLoadHandle)
	m_AsyncParticleLoadHandle->ReleaseHandle();

	TAssetPtr<UParticleSystem>	NewParticle(m_AsyncParticlePath); //경로를 넣어줌 로딩해둔 파티클을 얻어올수있게해줌

	if (NewParticle)
	{
		m_ParticleSystem->SetTemplate(NewParticle.Get());
		//m_ParticleSystem->OnSystemFinished.AddDynamic(this, &ANormalEffect::ParticleFinish);
	}
}


void ANormalEffect::LoadNiagaraAsync(const FString& Name)
{
	UAssetPathMain* AssetPath = UAssetPathMain::StaticClass()->GetDefaultObject<UAssetPathMain>();

	const FSoftObjectPath* Path = AssetPath->FindParticlePath(Name);

	if (!Path)
		return;

	//패스를 가지고 와서 일단 저장
	m_AsyncNiagaraPath = *Path;

	FStreamableManager& streamMgr = UAssetManager::GetStreamableManager();

	//인자를 넣어준다.... 호출될 함수를 지정해준다.(델리게이트처럼) 델리게이트름 만들어서 지정
	m_AsyncNiagaraLoadHandle = streamMgr.RequestAsyncLoad(m_AsyncNiagaraPath,
		FStreamableDelegate::CreateUObject(this, &ANormalEffect::LoadNiagaraAsyncComplete));
}

void ANormalEffect::LoadNiagaraAsyncComplete()
{
	//로딩이 끝났을 대 핸들을 릴리즈 해줌 아까 받아놨던것 (m_AsyncParticleLoadHandle)
	m_AsyncNiagaraLoadHandle->ReleaseHandle();

	TAssetPtr<UNiagaraSystem>	NewNiagara(m_AsyncNiagaraPath); //경로를 넣어줌 로딩해둔 파티클을 얻어올수있게해줌

	if (NewNiagara)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NewNiagara.Get(), GetActorLocation(), GetActorRotation());
}

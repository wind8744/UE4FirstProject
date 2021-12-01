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


	// OnSystemFinished : �� ��ƼŬ�ý���������Ʈ�� ������ �ִ� ��ƼŬ�ý����� �����
	// ��� ���ᰡ �Ǿ��� ��� ȣ��� �Լ��� �����صд�.
	// ����� ������ �Լ��� ȣ��ȴ�.
	if (!m_AssetLoop)
		m_ParticleSystem->OnSystemFinished.AddDynamic(this, &ANormalEffect::ParticleFinish); //�����϶� �Լ��� �θ��� ������


}

// Called every frame
void ANormalEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ANormalEffect::LoadNiagara(const FString& Path)
{
	//���̾ư���
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

	//�н��� ������ �ͼ� �ϴ� ����
	m_AsyncParticlePath = *Path;

	// UAssetManaget ->�𸮾󿡼� �����Ǵ� �Ŵ��� , ������ �������ִ� ���¸Ŵ����� �����(�̱������� �Ǿ��ִ�.)
	// �׳༮�� ��ü�� ��������, �񵿱⸦ �õ�
	FStreamableManager& streamMgr = UAssetManager::GetStreamableManager();

	//���ڸ� �־��ش�.... ȣ��� �Լ��� �������ش�.(��������Ʈó��) ��������Ʈ�� ���� ����
	m_AsyncParticleLoadHandle = streamMgr.RequestAsyncLoad(m_AsyncParticlePath,
		FStreamableDelegate::CreateUObject(this, &ANormalEffect::LoadParticleAsyncComplete));
}


void ANormalEffect::LoadParticleAsyncComplete()
{
	//�ε��� ������ �� �ڵ��� ������ ���� �Ʊ� �޾Ƴ����� (m_AsyncParticleLoadHandle)
	m_AsyncParticleLoadHandle->ReleaseHandle();

	TAssetPtr<UParticleSystem>	NewParticle(m_AsyncParticlePath); //��θ� �־��� �ε��ص� ��ƼŬ�� ���ü��ְ�����

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

	//�н��� ������ �ͼ� �ϴ� ����
	m_AsyncNiagaraPath = *Path;

	FStreamableManager& streamMgr = UAssetManager::GetStreamableManager();

	//���ڸ� �־��ش�.... ȣ��� �Լ��� �������ش�.(��������Ʈó��) ��������Ʈ�� ���� ����
	m_AsyncNiagaraLoadHandle = streamMgr.RequestAsyncLoad(m_AsyncNiagaraPath,
		FStreamableDelegate::CreateUObject(this, &ANormalEffect::LoadNiagaraAsyncComplete));
}

void ANormalEffect::LoadNiagaraAsyncComplete()
{
	//�ε��� ������ �� �ڵ��� ������ ���� �Ʊ� �޾Ƴ����� (m_AsyncParticleLoadHandle)
	m_AsyncNiagaraLoadHandle->ReleaseHandle();

	TAssetPtr<UNiagaraSystem>	NewNiagara(m_AsyncNiagaraPath); //��θ� �־��� �ε��ص� ��ƼŬ�� ���ü��ְ�����

	if (NewNiagara)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NewNiagara.Get(), GetActorLocation(), GetActorRotation());
}

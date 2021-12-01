// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "NormalEffect.generated.h"

class UNiagaraSystem;

UCLASS()
class CHUCHU_API ANormalEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANormalEffect();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* m_ParticleSystem;

	FSoftObjectPath		m_AsyncParticlePath;
	FSoftObjectPath		m_AsyncSoundPath;
	FSoftObjectPath		m_AsyncNiagaraPath;

	// 언리얼에서 제공하는 포인터
	// 동기화 : 작업을 하고있을때에는 다른것을 할 수가 없다. 
	// 비동기 : 일을 할 때 뒤에서 일을 시키고 앞에서 다른 작업을 할 숭 있다.
	//비동기가 끝났는지 어떠케 아냐면 FSTreambleHandle을 이용하여 포인터 사용
	TSharedPtr<FStreamableHandle>	m_AsyncParticleLoadHandle;
	TSharedPtr<FStreamableHandle>	m_AsyncSoundLoadHandle;
	TSharedPtr<FStreamableHandle>	m_AsyncNiagaraLoadHandle;

	bool	m_AssetLoop; //한번 사용 후 없앨건지 아닐건지

public:
	void AssetLoop()
	{
		m_AssetLoop = true;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void LoadNiagara(const FString& Path);// , const FVector& Location);
	void LoadParticle(const FString& Path); //파티클 경로를 읽어와서 지정해줌
	void LoadSound(const FString& Path);

	void LoadParticleAsync(const FString& Name);//이름으로 찾아서 로드를 할 수 있는 시스템
	void LoadParticleAsyncComplete();

	void LoadSoundAsync(const FString& Name);
	void LoadSoundAsyncComplete();

	void LoadNiagaraAsync(const FString& Name);
	void LoadNiagaraAsyncComplete();
public:
	UFUNCTION()
		void ParticleFinish(UParticleSystemComponent* Particle);

};

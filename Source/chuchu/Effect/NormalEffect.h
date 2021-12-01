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

	// �𸮾󿡼� �����ϴ� ������
	// ����ȭ : �۾��� �ϰ����������� �ٸ����� �� ���� ����. 
	// �񵿱� : ���� �� �� �ڿ��� ���� ��Ű�� �տ��� �ٸ� �۾��� �� �� �ִ�.
	//�񵿱Ⱑ �������� ��� �Ƴĸ� FSTreambleHandle�� �̿��Ͽ� ������ ���
	TSharedPtr<FStreamableHandle>	m_AsyncParticleLoadHandle;
	TSharedPtr<FStreamableHandle>	m_AsyncSoundLoadHandle;
	TSharedPtr<FStreamableHandle>	m_AsyncNiagaraLoadHandle;

	bool	m_AssetLoop; //�ѹ� ��� �� ���ٰ��� �ƴҰ���

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
	void LoadParticle(const FString& Path); //��ƼŬ ��θ� �о�ͼ� ��������
	void LoadSound(const FString& Path);

	void LoadParticleAsync(const FString& Name);//�̸����� ã�Ƽ� �ε带 �� �� �ִ� �ý���
	void LoadParticleAsyncComplete();

	void LoadSoundAsync(const FString& Name);
	void LoadSoundAsyncComplete();

	void LoadNiagaraAsync(const FString& Name);
	void LoadNiagaraAsyncComplete();
public:
	UFUNCTION()
		void ParticleFinish(UParticleSystemComponent* Particle);

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawnPoint.h"
#include "PatrolPoint.h"

// Sets default values
AMonsterSpawnPoint::AMonsterSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	m_Monster = nullptr;

	m_SpawnTime = 1.f;
	m_AccTime = 0.f;
}

// Called when the game starts or when spawned
void AMonsterSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	if (m_MonsterClass) //������ ���� Ŭ������ ������ �� ������Ű��
	{
		FActorSpawnParameters	param; //�浹�� �Ǹ� �ȵǴ� ��ġ�� ã�Ƽ� �������Ѿ� �ϹǷ� �� �Ķ���͸� �ҷ��ͼ� ����.
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AMonster* Monster = GetWorld()->SpawnActor<AMonster>(m_MonsterClass,
			GetActorLocation(), GetActorRotation(), param); //���� ����

		Monster->SetSpawnPoint(this);

		//������ �� �ڽ��� ��ġ�� ��Ʈ�� ����Ʈ�� ���� �־��ش�
		Monster->AddPatrolPoint(GetActorLocation());

		//���� ���ʴ�� ���� ��Ʈ�� pos�� �־��ش�.
		for (auto& Point : m_PatrolPointArray)
		{
			Monster->AddPatrolPoint(Point->GetActorLocation());
		}

		m_Monster = Monster;
	}
}

// Called every frame
void AMonsterSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_Monster)
	{
		m_AccTime += DeltaTime;

		if (m_AccTime >= m_SpawnTime) //�����ð��� �Ǹ�
		{
			m_AccTime = 0.f;

			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AMonster* Monster = GetWorld()->SpawnActor<AMonster>(m_MonsterClass,
				GetActorLocation(), GetActorRotation(), param);

			Monster->SetSpawnPoint(this);

			Monster->AddPatrolPoint(GetActorLocation());

			for (auto& Point : m_PatrolPointArray)
			{
				Monster->AddPatrolPoint(Point->GetActorLocation());
			}
			 
			m_Monster = Monster;
		}
	}
}


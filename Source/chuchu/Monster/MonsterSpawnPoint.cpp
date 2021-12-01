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

	if (m_MonsterClass) //지정된 몬스터 클래스가 존재할 때 스폰시키기
	{
		FActorSpawnParameters	param; //충돌이 되면 안되는 위치를 찾아서 스폰시켜야 하므로 그 파라미터를 불러와서 쓴다.
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AMonster* Monster = GetWorld()->SpawnActor<AMonster>(m_MonsterClass,
			GetActorLocation(), GetActorRotation(), param); //스폰 액터

		Monster->SetSpawnPoint(this);

		//스폰될 때 자신의 위치를 패트롤 포인트로 먼저 넣어준다
		Monster->AddPatrolPoint(GetActorLocation());

		//다음 차례대로 액터 패트롤 pos를 넣어준다.
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

		if (m_AccTime >= m_SpawnTime) //스폰시간이 되면
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


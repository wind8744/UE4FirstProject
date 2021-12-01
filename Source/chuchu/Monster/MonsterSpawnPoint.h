// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawnPoint.generated.h"

// ���� ���� ��Ҹ� �������ִ� Ŭ����
UCLASS()
class CHUCHU_API AMonsterSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawnPoint();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AMonster>	m_MonsterClass; // ���� ������Ű���� �ϴ� Ŭ������ ������ �־�� �� �� ���� (���忡 ���͸� �������Ѿ��� �� Ŭ������ �ʿ��ϹǷ�)

	AMonster* m_Monster; //���� ������ ���� �� ��� ���ʹ� �ڽ��� ���� ����Ʈ�� �˰� ���� 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	m_SpawnTime; //���� Ÿ��

	float	m_AccTime;

	//TArray�� �𸮾󿡼� �����ϴ� �迭 Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<class APatrolPoint*>	m_PatrolPointArray; //��Ʈ�� �迭 �ϳ� ����
	//��Ʈ�� ���͸� �ʿ� �� ���� ���Ͱ� ������ �� �� ��������Ʈ�� �����ִ� ��Ʈ���� ��ġ������ ���� �� �ְ�

public:
	void Death()
	{
		m_Monster = nullptr;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

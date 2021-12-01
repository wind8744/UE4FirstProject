// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawnPoint.generated.h"

// 몬스터 스폰 장소를 관리해주는 클래스
UCLASS()
class CHUCHU_API AMonsterSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawnPoint();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AMonster>	m_MonsterClass; // 내가 생성시키고자 하는 클래스를 가지고 있어야 할 때 선언 (월드에 액터를 생성시켜야할 때 클래스가 필요하므로)

	AMonster* m_Monster; //실제 생성한 몬스터 ㅡ 모든 몬스터는 자신의 생성 포인트를 알고 있음 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	m_SpawnTime; //스폰 타임

	float	m_AccTime;

	//TArray는 언리얼에서 제공하는 배열 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<class APatrolPoint*>	m_PatrolPointArray; //패트롤 배열 하나 생성
	//패트롤 액터를 맵에 깐 다음 몬스터가 스폰이 될 때 스폰포인트가 갖고있는 패트롤의 위치정보를 가질 수 있게

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

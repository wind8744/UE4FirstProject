// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "MonsterAnimInstance.h"
#include "Monster.generated.h"


USTRUCT(BlueprintType)
struct FMonsterInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString			Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			HPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			MPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			AttackAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			TraceDistance;
};



UCLASS()
class CHUCHU_API AMonster : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AMonster();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FMonsterInfo	m_MonsterInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* m_HPBar;

	class UHPBar* m_HPBarWidget;

	class AMonsterSpawnPoint* m_SpawnPoint; //몬스터는 자신의 스폰포인트를 알아야 함, 생성될 때는 널

	FString			m_MonsterInfoName;

	class UMonsterAnimInstance* m_AnimInstance;

	bool		m_AttackEnd;

	//패트롤 액터를 맵에 깐 다음 몬스터가 스폰이 될 때 스폰포인트가 갖고있는 패트롤의 위치정보를 가질 수 있게
	TArray<FVector>	m_PatrolArray;
	int32			m_PatrolIndex;

	bool		m_DissolveEnable;
	float		m_Dissolve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float		m_DissolveMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float		m_DissolveMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float		m_DissolveTime;

	float		m_DissolveAccTime;
	float		m_DissolveRange;


	TArray<FString> m_DropItemArray; //드랍 아이템 목록

public:
	FVector GetPatrolPoint()
	{
		if (m_PatrolIndex >= m_PatrolArray.Num())
			m_PatrolIndex = 0;

		return m_PatrolArray[m_PatrolIndex];
	}

	void NextPatrolPoint()
	{
		++m_PatrolIndex;

		if (m_PatrolIndex >= m_PatrolArray.Num())
			m_PatrolIndex = 0;
	}

	EMonsterAnimType GetAnimType()	const;

	bool GetAttackEnd()	const
	{
		return m_AttackEnd;
	}

	void SetAttackEnd(bool AttackEnd)
	{
		m_AttackEnd = AttackEnd;
	}

	void ChangeAnimType(EMonsterAnimType Type);

	void SetSpawnPoint(class AMonsterSpawnPoint* SpawnPoint)
	{
		m_SpawnPoint = SpawnPoint;
	}

	FMonsterInfo& GetMonsterInfo()
	{
		return m_MonsterInfo;
	}

	void AddPatrolPoint(const FVector& Point)
	{
		m_PatrolArray.Add(Point); //몬스터가 갖고있는 패트롤array에 패트롤pos를 넣어준다
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//데미지를 줄 수 있는 함수 재정의
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);


public:
	void Death();

public:
	virtual void NormalAttack();
	virtual void SpawnEnd();


public:
	UFUNCTION()
		void NameWidgetCallback();
};

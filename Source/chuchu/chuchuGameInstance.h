// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "Engine/GameInstance.h"
#include "chuchuGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FMonsterTableInfo :
	public FTableRowBase //�긦 ��ӹ޾ƾ� ������ ���̺�� ����ü�� ����� �� �յ�.
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


USTRUCT(BlueprintType)
struct FPlayerTableInfo :
	public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString			Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EPlayerJob			Job;

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
};

UCLASS()
class CHUCHU_API UchuchuGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UchuchuGameInstance();

private:
	UPROPERTY()
		UDataTable* m_MonsterInfoTable; //�������� ������ ���� �����ϱ� ����

	UPROPERTY()
		UDataTable* m_PlayerInfoTable;

	UPROPERTY()
		UDataTable* m_UIItemInfoTable;

	UPROPERTY()
		UDataTable* m_QuestTableInfo;

	EPlayerJob	m_SelectJob;
	FString		m_SelectName; //����

	TMap<FString, FQuestData> m_QuestComplete;

public:

	const FString& GetPlayerInfoName() { return m_SelectName; } //����
	void SetPlayerInfoName(const FString& Job, const FString& Name);  //����

	void SetSelectJob(EPlayerJob Job)
	{
		m_SelectJob = Job;
	}

	EPlayerJob GetSelectJob()	const
	{
		return m_SelectJob;
	}

	const TMap<FString, FQuestData>& GetQuestData() { return m_QuestComplete; }

public:
	virtual void Init();
	
public:
	const FMonsterTableInfo* FindMonsterInfo(const FString& Name);
	const FPlayerTableInfo* FindPlayerInfo(const FString& Name);
	const FUIItemDataInfo* FindUIItemInfo(const FString& Name);
	const FQuestTableInfo* FindQuestTableInfo(const FString& Name);
};

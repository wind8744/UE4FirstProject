// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"
#include "DestructibleComponent.h"
#include "Engine.h"
#include "EngineGlobals.h"
#include "Engine/AssetManager.h"
#include "Engine/Public/TimerManager.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h" //���콺 ���� ���
#include "NiagaraComponent.h" 
#include "NiagaraFunctionLibrary.h"
#include "Math/UnrealMathVectorCommon.h" 
//#include "Engine/Source/Runtime/Core/Public/Math/UnrealMathVectorCommon.h" //�𸮾� ������ ����� �ƴ϶� ��Ŭ��常 ���� ��

#include "GameInfo.generated.h"

/*
FString : �𸮾������� �����Ǵ� ���ڿ� Ÿ���̴�.
FString�� �տ� * �� �ٿ��ָ� �� FString�� ���� ������ �ִ� ���ڿ��� �ּҸ� �ǹ��Ѵ�.

__FUNCTION__ : ���� �� �α׸� ����ϴ� �Լ� �̸��� �ǹ��Ѵ�.
__LINE__ : ���� �ڵ��� �ٹ�ȣ�� �ǹ��Ѵ�.
*/

//�α׿� ����ϱ� ���� �غ� 8/ 17 12:50
DECLARE_LOG_CATEGORY_EXTERN(chuchu, Log, All);
#define	LOG_CALLINFO	(FString(__FUNCTION__) + TEXT("{") + FString::FromInt(__LINE__) + TEXT("}"))
#define	LOG(Format, ...)	UE_LOG(chuchu, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define	LOGSTRING(Str)		UE_LOG(chuchu, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *Str)

//�����Ϳ� ����غ��� ���
void PrintViewport(float Time, const FColor& Color, const FString& Text);
FString GetEnumToString(EEquipType value);
FString GetEnumToString(EPlayerAnimType value);

UENUM(BlueprintType) //�������Ʈ�� ����� �� �ִ°��� �˷��־�� ��
enum class EPlayerAnimType : uint8
{
	Ground,
	Jump,
	Fall,
	Death,
	Avoid,
	Equip, //equip���� �����
	Skill,
};

// �÷��̾��� ������ ������ ���°� ���ŷο�Ƿ� ����ü�� ���� ���
UENUM(BlueprintType)
enum class EPlayerJob : uint8
{
	Knight,
	Archer,
	Magicion,
	End
};

UENUM(BlueprintType)
enum class ECheckDistanceType : uint8
{
	Trace,
	Attack
};

UENUM(BlueprintType)
enum class ETriggerType : uint8
{
	Box,
	Sphere,
	Capsule
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Equip,
	Food
};
UENUM(BlueprintType)
enum class EEquipType : uint8
{
	HAIR,
	TOPBODY,
	BOTBODY,
	HAND,
	BOOTS,
	WEAPON,
	NONE
};

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY() //�⺻���� ���� ����� ����

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString			Name; //ĳ���� �̸�����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EPlayerJob		Job;

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

USTRUCT(BlueprintType)
struct FUIItemDataInfo :
	public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString	m_ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EItemType	m_ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTexture2D* m_Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString	m_ThumbnailPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMesh* m_PickMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString m_ItemDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EEquipType m_EquipType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString m_MeshPath;
};

USTRUCT()
struct FAssetPathInfo
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, Category = "Path")
		FString		Name;

	UPROPERTY(EditAnywhere, Category = "Path")
		FSoftObjectPath		Path;
};

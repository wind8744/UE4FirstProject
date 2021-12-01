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
#include "Blueprint/SlateBlueprintLibrary.h" //마우스 관련 헤더
#include "NiagaraComponent.h" 
#include "NiagaraFunctionLibrary.h"
#include "Math/UnrealMathVectorCommon.h" 
//#include "Engine/Source/Runtime/Core/Public/Math/UnrealMathVectorCommon.h" //언리얼 문서의 헤더가 아니라 인클루드만 쓰면 됨

#include "GameInfo.generated.h"

/*
FString : 언리얼엔진에서 제공되는 문자열 타입이다.
FString은 앞에 * 을 붙여주면 이 FString이 실제 가지고 있는 문자열의 주소를 의미한다.

__FUNCTION__ : 현재 이 로그를 출력하는 함수 이름을 의미한다.
__LINE__ : 현재 코드의 줄번호를 의미한다.
*/

//로그에 출력하기 위한 준비 8/ 17 12:50
DECLARE_LOG_CATEGORY_EXTERN(chuchu, Log, All);
#define	LOG_CALLINFO	(FString(__FUNCTION__) + TEXT("{") + FString::FromInt(__LINE__) + TEXT("}"))
#define	LOG(Format, ...)	UE_LOG(chuchu, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define	LOGSTRING(Str)		UE_LOG(chuchu, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *Str)

//에디터에 출력해보는 방법
void PrintViewport(float Time, const FColor& Color, const FString& Text);
FString GetEnumToString(EEquipType value);

// 플레이어의 정보를 일일히 쓰는건 번거로우므로 구조체를 만들어서 사용
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
	GENERATED_BODY() //기본적인 것을 만들기 위해

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString			Name; //캐릭터 이름정보

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

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "SelectPlayerAnimInstance.generated.h"
UENUM(BlueprintType)
enum class ESelectPlayerAnimType : uint8
{
	Idle,
	Select
};
UCLASS()
class CHUCHU_API USelectPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	USelectPlayerAnimInstance();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ESelectPlayerAnimType	m_AnimType;

public:
	void ChangeAnimType(ESelectPlayerAnimType Type)
	{
		m_AnimType = Type;
	}

public:
	UFUNCTION()
		void AnimNotify_TransitionIdle();
};

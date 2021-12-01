// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckDistance.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API UBTDecorator_CheckDistance : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckDistance();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ECheckDistanceType	m_CheckType; //�����Ÿ����� ���ݰŸ�����

protected:
	//����üũ�� ������ �ѹ��� ����
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};

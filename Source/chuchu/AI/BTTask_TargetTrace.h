// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TargetTrace.generated.h"

/**
 * BTtask class�� ���ϴ� �ൿ�� �����ų���ִ�.
 */
UCLASS()
class CHUCHU_API UBTTask_TargetTrace : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TargetTrace();
	~UBTTask_TargetTrace();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory); //�׽�ũ �ߴܵɶ� ������ ��
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds); //�� �����Ӹ��� ����, ����� � ������ ������ .. 
};

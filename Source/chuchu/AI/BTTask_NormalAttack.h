// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_NormalAttack.generated.h"

/**
 * BTtask class는 원하는 행동을 수행시킬수있다.
 */
UCLASS()
class CHUCHU_API UBTTask_NormalAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_NormalAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory); //테스크 중단될때 들어오는 놈
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds); //몇 프래임마다 들어옴, 여기다 어떤 연산을 해줄지 .. 
};

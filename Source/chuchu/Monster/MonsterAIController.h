// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterAIController();

protected:
 	//자기가 사용할 인공지능 에셋들을 들고있게 함
	//블랙보드랑 비헤이브 트리로 만들어짐 
	//몬스터마다 다른 트리랑 보드를 가지루수 있듬
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true")) //수정할 필요가 없기 때문에 visible
		UBehaviorTree* m_AITree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBlackboardData* m_AIBlackboard;

protected:
	virtual void OnPossess(APawn* InPawn) override; //ai컨트롤러가 해당 폰에 (ai)에 빙의될때
	virtual void OnUnPossess() override; //ai컨트롤러가 ai에 빙의 해제될때 
};

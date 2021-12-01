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
 	//�ڱⰡ ����� �ΰ����� ���µ��� ����ְ� ��
	//������� �����̺� Ʈ���� ������� 
	//���͸��� �ٸ� Ʈ���� ���带 ������� �ֵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true")) //������ �ʿ䰡 ���� ������ visible
		UBehaviorTree* m_AITree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBlackboardData* m_AIBlackboard;

protected:
	virtual void OnPossess(APawn* InPawn) override; //ai��Ʈ�ѷ��� �ش� ���� (ai)�� ���ǵɶ�
	virtual void OnUnPossess() override; //ai��Ʈ�ѷ��� ai�� ���� �����ɶ� 
};

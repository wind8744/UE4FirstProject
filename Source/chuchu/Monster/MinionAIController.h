// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MonsterAIController.h"
#include "MinionAIController.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API AMinionAIController : public AMonsterAIController
{
	GENERATED_BODY()

public:
	AMinionAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override; //ai��Ʈ�ѷ��� �ش� ���� (ai)�� ���ǵɶ�
	virtual void OnUnPossess() override; //ai��Ʈ�ѷ��� ai�� ���� �����ɶ� 
};

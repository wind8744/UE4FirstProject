// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "MonsterAIController.h"
#include "BuffAIController.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API ABuffAIController : public AMonsterAIController
{
	GENERATED_BODY()

public:
	ABuffAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override; //ai��Ʈ�ѷ��� �ش� ���� (ai)�� ���ǵɶ�
	virtual void OnUnPossess() override; //ai��Ʈ�ѷ��� ai�� ���� �����ɶ� 

};
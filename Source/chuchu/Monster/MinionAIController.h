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
	virtual void OnPossess(APawn* InPawn) override; //ai컨트롤러가 해당 폰에 (ai)에 빙의될때
	virtual void OnUnPossess() override; //ai컨트롤러가 ai에 빙의 해제될때 
};

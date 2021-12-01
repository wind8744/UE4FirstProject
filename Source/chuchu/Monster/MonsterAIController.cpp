// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"

AMonsterAIController::AMonsterAIController()
{

}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// �����带 �����Ѵ�.
	if (UseBlackboard(m_AIBlackboard, Blackboard))
	{
		// �ൿƮ���� ���۽�Ų��.
		if (!RunBehaviorTree(m_AITree))
		{
			LOG(TEXT("Monster BehaviorTree Error!!"));
		}
	}
}

void AMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffAIController.h"

ABuffAIController::ABuffAIController()
{
	// ai������� ���ñ� Ʈ�� ���� �����
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>	AITreeAsset(TEXT("BehaviorTree'/Game/Monster/BTBuff.BTBuff'"));

	if (AITreeAsset.Succeeded())
		m_AITree = AITreeAsset.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData>	BlackboardAsset(TEXT("BlackboardData'/Game/Monster/BBMonster.BBMonster'"));

	if (BlackboardAsset.Succeeded())
		m_AIBlackboard = BlackboardAsset.Object;
}

void ABuffAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ABuffAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

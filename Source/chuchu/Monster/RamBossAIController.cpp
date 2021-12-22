// Fill out your copyright notice in the Description page of Project Settings.


#include "RamBossAIController.h"


ARamBossAIController::ARamBossAIController()
{
	// ai������� ���ñ� Ʈ�� ���� �����
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>	AITreeAsset(TEXT("BehaviorTree'/Game/Monster/BTRamBoss.BTRamBoss'"));

	if (AITreeAsset.Succeeded())
		m_AITree = AITreeAsset.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData>	BlackboardAsset(TEXT("BlackboardData'/Game/Monster/BBMonster.BBMonster'"));

	if (BlackboardAsset.Succeeded())
		m_AIBlackboard = BlackboardAsset.Object;
}

void ARamBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ARamBossAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

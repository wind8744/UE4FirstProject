// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossSkill1.h"
#include "../Player/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../Monster/RamBossAIController.h"

UBTTask_BossSkill1::UBTTask_BossSkill1()
{
	NodeName = TEXT("BossSKill1"); //��� ���� ����
	bNotifyTick = true; //ƽ�� ���� �� �ֵ���
}

// task�� ���۵� �� ����
EBTNodeResult::Type UBTTask_BossSkill1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ARamBossAIController* Controller = Cast<ARamBossAIController>(OwnerComp.GetAIOwner()); //���� ��Ʈ�ѷ��� ĳ������

	if (!Controller)
		return EBTNodeResult::Failed;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn()); //��Ʈ�ѷ��� �����ϴ� ���͸� ���´�.

	if (!Monster)
		return EBTNodeResult::Failed; 

	APlayerCharacter* Target = Cast<APlayerCharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target"))); // Ÿ�� �����

	if (!Target) //Ÿ���� ������ ���� ���� �̵�
	{
		Monster->ChangeAnimType(EMonsterAnimType::Idle);

		Controller->StopMovement();
		return EBTNodeResult::Failed;
	}

	int32 SKillNum = Monster->GetSKillNum();

	Monster->ChangeAnimType((EMonsterAnimType)(SKillNum+6)); //���� ����

	return EBTNodeResult::InProgress;
}

// �����Ӹ��� ����
void UBTTask_BossSkill1::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// InProgress�� �صξ��� ������ ���⼭ �ȳ�����.
	// �׷��� ���ݰŸ� �ȿ� ���Դ����� ���⼭�� üũ�Ͽ� ���ٸ�
	// Trace�� �����Ų��.
	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//debug
	//PrintViewport(1.f, FColor::Blue, TEXT("Skill1 Start"));


	APlayerCharacter* Target = Cast<APlayerCharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (!Target)
	{
		Monster->ChangeAnimType(EMonsterAnimType::Idle);

		Controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const FMonsterInfo& MonsterInfo = Monster->GetMonsterInfo();

	// Ÿ�ٰ��� �Ÿ��� ���Ѵ�.
	FVector	MonsterLoc = Monster->GetActorLocation();
	FVector	TargetLoc = Target->GetActorLocation();
	MonsterLoc.Z = TargetLoc.Z;
	float	Distance = FVector::Distance(MonsterLoc, TargetLoc);

	if (Monster->GetAttackEnd()) //������ �� ���������̸�
	{
		FVector	Dir = TargetLoc - MonsterLoc;
		Dir.Normalize();

		//Ÿ�� �������� ���� ȸ��
		Monster->SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
		Monster->SetAttackEnd(false); //���� �ʱ�ȭ

		Monster->ChangeAnimType(EMonsterAnimType::Idle);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	
	//if (Distance > MonsterInfo.AttackDistance) // ������ �Ÿ����� �ָ� ������ ������ ������
	//{
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	//}
	

}

// task�� �ߴܵ� �� ����
EBTNodeResult::Type UBTTask_BossSkill1::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}


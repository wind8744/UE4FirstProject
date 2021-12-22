// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossSkill1.h"
#include "../Player/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../Monster/RamBossAIController.h"

UBTTask_BossSkill1::UBTTask_BossSkill1()
{
	NodeName = TEXT("BossSKill1"); //노드 네임 지정
	bNotifyTick = true; //틱에 들어올 수 있도록
}

// task가 시작될 때 들어옴
EBTNodeResult::Type UBTTask_BossSkill1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ARamBossAIController* Controller = Cast<ARamBossAIController>(OwnerComp.GetAIOwner()); //보스 컨트롤러를 캐스팅함

	if (!Controller)
		return EBTNodeResult::Failed;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn()); //컨트롤러가 조종하는 몬스터를 들고온다.

	if (!Monster)
		return EBTNodeResult::Failed; 

	APlayerCharacter* Target = Cast<APlayerCharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target"))); // 타겟 갖고옴

	if (!Target) //타겟이 없으면 다음 노드로 이동
	{
		Monster->ChangeAnimType(EMonsterAnimType::Idle);

		Controller->StopMovement();
		return EBTNodeResult::Failed;
	}

	int32 SKillNum = Monster->GetSKillNum();

	Monster->ChangeAnimType((EMonsterAnimType)(SKillNum+6)); //공격 시작

	return EBTNodeResult::InProgress;
}

// 프레임마다 들어옴
void UBTTask_BossSkill1::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// InProgress로 해두었기 때문에 여기서 안나간다.
	// 그래서 공격거리 안에 들어왔는지를 여기서도 체크하여 들어갔다면
	// Trace를 종료시킨다.
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

	// 타겟과의 거리를 구한다.
	FVector	MonsterLoc = Monster->GetActorLocation();
	FVector	TargetLoc = Target->GetActorLocation();
	MonsterLoc.Z = TargetLoc.Z;
	float	Distance = FVector::Distance(MonsterLoc, TargetLoc);

	if (Monster->GetAttackEnd()) //공격이 다 끝난상태이면
	{
		FVector	Dir = TargetLoc - MonsterLoc;
		Dir.Normalize();

		//타겟 방향으로 몬스터 회전
		Monster->SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
		Monster->SetAttackEnd(false); //공격 초기화

		Monster->ChangeAnimType(EMonsterAnimType::Idle);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	
	//if (Distance > MonsterInfo.AttackDistance) // 정해진 거리보다 멀리 있으면 공격을 끝낸다
	//{
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	//}
	

}

// task가 중단될 때 들어옴
EBTNodeResult::Type UBTTask_BossSkill1::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}


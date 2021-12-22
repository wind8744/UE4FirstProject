// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckDistance.h"
#include "../Player/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAIController.h"

UBTDecorator_CheckDistance::UBTDecorator_CheckDistance()
{
	NodeName = TEXT("CheckDistance");
}

// 플레이어가 몬스터의 공격 범위 내에 있는지 아닌지  
// true면  Attack Task 실행
// false면 Target Trace Task 실행
bool UBTDecorator_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	//몬스터 얻어옴
	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return false;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return false;

	//타겟 얻어옴
	APlayerCharacter* Target = Cast<APlayerCharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!Target)
		return false;
	
	//몬스터 인포
	const FMonsterInfo& MonsterInfo = Monster->GetMonsterInfo();

	// 타겟과의 거리ㅌㅌㅌ를 구한다.
	FVector	MonsterLoc = Monster->GetActorLocation();
	FVector	TargetLoc = Target->GetActorLocation();

	MonsterLoc.Z = TargetLoc.Z; //z높이차를 빼고 타겟 거리를 구한다 (몬스터 크기가 플레이어랑 차이날 수도 있으므로)

	//높이를 똑같이 만든 후 xy평면ㄴ상의 거리를 구함
	float	Distance = FVector::Distance(MonsterLoc, TargetLoc);
	float	CheckDist = 0.f;

	switch (m_CheckType)
	{
	case ECheckDistanceType::Trace:
		CheckDist = MonsterInfo.TraceDistance;
		break;
	case ECheckDistanceType::Attack:
		CheckDist = MonsterInfo.AttackDistance;
		break;
	}

	return Distance <= CheckDist;
}

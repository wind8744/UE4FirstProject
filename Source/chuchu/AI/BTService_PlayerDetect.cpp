// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerDetect.h"
#include "../Player/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAIController.h"

UBTService_PlayerDetect::UBTService_PlayerDetect()
{
	NodeName = TEXT("Detect"); //노드네임
	Interval = 0.5f; //호출되는 시간 0.5초마다 호출
}

//틱이 돌때마다 들어옴
void UBTService_PlayerDetect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory,float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	//컨트롤러를 들고
	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return;
	//몬스터를 들고 (자신이 빙의도오있는 폰)
	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster) //몬스터가아니면 리턴
		return;
 
	//몬스터 정보를 얻어와서
	const FMonsterInfo& MonsterInfo = Monster->GetMonsterInfo();

	// 주변에 플레이어가 있는지 판단한다.
	FCollisionQueryParams	params(NAME_None, false, Monster);

	// 근접공격으로 이 타이밍에 충돌처리를 해주도록 한다.
	FHitResult	HitResult; //제일 가까운놈 한마리만 찾는다
	bool Sweep = GetWorld()->SweepSingleByChannel(HitResult, //한마리니까 single
		Monster->GetActorLocation(), //몬스터위치
		Monster->GetActorLocation(), FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6, //콜리전에서 플레이어디텍트 트레이스로 하나 만들고 , defaultEngine.ini에서 파라미터 찾아옴
		FCollisionShape::MakeSphere(MonsterInfo.TraceDistance),
		params);

#if ENABLE_DRAW_DEBUG

	//FColor	DrawColor = Sweep ? FColor::Red : FColor::Green; //층돌이 있으면 red

	//DrawDebugSphere(GetWorld(), Monster->GetActorLocation(),
	//	MonsterInfo.TraceDistance, 20, DrawColor, false,
	//	0.3f);

#endif

	if (Sweep) //충돌이 있으면
		//컨트롤러는 블랙보드 들고있음 , 부딪힌 액터를 얻어몸
	{
		Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), HitResult.GetActor());
	}

	else //부딪힌 넘이 없으므로 null
	{
		Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
	}
		
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerDetect.h"
#include "../Player/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAIController.h"

UBTService_PlayerDetect::UBTService_PlayerDetect()
{
	NodeName = TEXT("Detect"); //������
	Interval = 0.5f; //ȣ��Ǵ� �ð� 0.5�ʸ��� ȣ��
}

//ƽ�� �������� ����
void UBTService_PlayerDetect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory,float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	//��Ʈ�ѷ��� ���
	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return;
	//���͸� ��� (�ڽ��� ���ǵ����ִ� ��)
	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster) //���Ͱ��ƴϸ� ����
		return;
 
	//���� ������ ���ͼ�
	const FMonsterInfo& MonsterInfo = Monster->GetMonsterInfo();

	// �ֺ��� �÷��̾ �ִ��� �Ǵ��Ѵ�.
	FCollisionQueryParams	params(NAME_None, false, Monster);

	// ������������ �� Ÿ�ֿ̹� �浹ó���� ���ֵ��� �Ѵ�.
	FHitResult	HitResult; //���� ������ �Ѹ����� ã�´�
	bool Sweep = GetWorld()->SweepSingleByChannel(HitResult, //�Ѹ����ϱ� single
		Monster->GetActorLocation(), //������ġ
		Monster->GetActorLocation(), FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6, //�ݸ������� �÷��̾����Ʈ Ʈ���̽��� �ϳ� ����� , defaultEngine.ini���� �Ķ���� ã�ƿ�
		FCollisionShape::MakeSphere(MonsterInfo.TraceDistance),
		params);

#if ENABLE_DRAW_DEBUG

	//FColor	DrawColor = Sweep ? FColor::Red : FColor::Green; //������ ������ red

	//DrawDebugSphere(GetWorld(), Monster->GetActorLocation(),
	//	MonsterInfo.TraceDistance, 20, DrawColor, false,
	//	0.3f);

#endif

	if (Sweep) //�浹�� ������
		//��Ʈ�ѷ��� ������ ������� , �ε��� ���͸� ����
	{
		Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), HitResult.GetActor());
	}

	else //�ε��� ���� �����Ƿ� null
	{
		Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
	}
		
}

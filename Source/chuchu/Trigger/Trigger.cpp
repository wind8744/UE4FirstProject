// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger.h"

// Sets default values
ATrigger::ATrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrigger::BeginPlay()
{
	Super::BeginPlay();

	// 함수 호출 등록
	m_Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATrigger::TriggerBeginOverlap);
	m_Trigger->OnComponentEndOverlap.AddDynamic(this, &ATrigger::TriggerEndOverlap);
}

// Called every frame
void ATrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrigger::TriggerBeginOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//트리거가 오버랩 돼서 들어오는 이벤트에 델리게이트를 브로캐스트 하면 안에 등록된 모든 함수가 호출 된다 -> 트리거를 상속받은 포탈 트리거클래스에서 트리거 비긴 함수가 실행된다
	//PrintViewport(1.f, FColor::Red, TEXT("Broadcast !!"));
	m_DelegateTriggerBegin.Broadcast(); 

}

void ATrigger::TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex)
{
	//PrintViewport(1.f, FColor::Red, TEXT("P End Trigger"));
	m_DelegateTriggerEnd.Broadcast();
}

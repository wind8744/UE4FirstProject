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

//강의 55 참고
void ATrigger::TriggerBeginOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	//PrintViewport(1.f, FColor::Red, TEXT("P Begin Trigger"));
	m_TriggerBeginDelegate.Broadcast();
}

void ATrigger::TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	//PrintViewport(1.f, FColor::Red, TEXT("P End Trigger"));
	m_TriggerEndDelegate.Broadcast();
}

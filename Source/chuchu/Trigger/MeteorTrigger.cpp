// Fill out your copyright notice in the Description page of Project Settings.


#include "MeteorTrigger.h"
#include "../Skill/Meteor.h"

// Sets default values
AMeteorTrigger::AMeteorTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMeteorTrigger::BeginPlay()
{
	Super::BeginPlay(); //obj 클래스는 meteor라서 이 함수에 먼저 들어오고 다시 부모 함수로 들어간 후 끝나면 자식(여기)로 실행 parent -> child 

	m_TriggerBeginDelegate.AddDynamic(this, &AMeteorTrigger::TriggerBegin);
	m_TriggerEndDelegate.AddDynamic(this, &AMeteorTrigger::TriggerEnd);
}

// Called every frame
void AMeteorTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeteorTrigger::TriggerBegin()
{
	//PrintViewport(1.f, FColor::Red, TEXT("c Begin Trigger"));

	//파라미터 
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//트리거가 발생 시작에 들어왔을때
	//skill에서 불타는 감자 만들어놓은것 스폰
	AMeteor* Meteor = GetWorld()->SpawnActor<AMeteor>(AMeteor::StaticClass(),
		GetActorLocation() + FVector(0.f, 0.f, 500.f),
		FRotator::ZeroRotator, param);
}

void AMeteorTrigger::TriggerEnd()
{
	//PrintViewport(1.f, FColor::Red, TEXT("c End Trigger"));
}


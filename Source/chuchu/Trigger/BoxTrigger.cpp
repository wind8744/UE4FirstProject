// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxTrigger.h"

// Sets default values
ABoxTrigger::ABoxTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//모양별로 컴포넌트가 만들어짐 (트리거에 부팆혔을대 사용하기 위해)
	m_Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	//트리거에 부딪혔을때 호출될 함수를 만들어준다.
	//원하는 모양을 상속받아 작업하면 된다.
	//델리게이트 또는 멀티캐스트로 함수 호출되도록

	SetRootComponent(m_Trigger);

	m_ShapeType = ETriggerType::Box;
}

// Called when the game starts or when spawned
void ABoxTrigger::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABoxTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


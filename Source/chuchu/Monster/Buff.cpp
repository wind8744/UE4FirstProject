// Fill out your copyright notice in the Description page of Project Settings.


#include "Buff.h"
#include "BuffAIController.h"

// Sets default values
ABuff::ABuff()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 그냥 부모 MOnsterAI컨트롤러가 기본이 되어이쓰으모로 바꺼준다
	AIControllerClass = ABuffAIController::StaticClass();


	// 드랍 아이템
	m_DropItemArray.Add(TEXT("GreatSword"));
}

// Called when the game starts or when spawned
void ABuff::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABuff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

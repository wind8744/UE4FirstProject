// Fill out your copyright notice in the Description page of Project Settings.


#include "Buff.h"
#include "BuffAIController.h"

// Sets default values
ABuff::ABuff()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �׳� �θ� MOnsterAI��Ʈ�ѷ��� �⺻�� �Ǿ��̾������ �ٲ��ش�
	AIControllerClass = ABuffAIController::StaticClass();
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

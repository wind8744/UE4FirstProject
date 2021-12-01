// Fill out your copyright notice in the Description page of Project Settings.


#include "Minion.h"
#include "MinionAIController.h"

// Sets default values
AMinion::AMinion()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//미니언은 이것의(AMinionAIController) 클래스를 이용해서 ai컨트롤러를 만들고 그걸로 빙의해라 라는 함수, 그냥 부모 MOnsterAI컨트롤러가 기본이 되어이쓰으모로 바꺼준다
	AIControllerClass = AMinionAIController::StaticClass();
}

// Called when the game starts or when spawned
void AMinion::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

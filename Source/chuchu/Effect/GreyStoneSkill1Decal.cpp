// Fill out your copyright notice in the Description page of Project Settings.


#include "GreyStoneSkill1Decal.h"

// Sets default values
AGreyStoneSkill1Decal::AGreyStoneSkill1Decal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterial>	DecalMtrlAsset(TEXT("Material'/Game/Player/GreyStone/MTGreyStoneSkill1.MTGreyStoneSkill1'"));

	if (DecalMtrlAsset.Succeeded())
		m_Decal->SetDecalMaterial(DecalMtrlAsset.Object);
}

// Called when the game starts or when spawned
void AGreyStoneSkill1Decal::BeginPlay()
{
	Super::BeginPlay();
	//언리얼에서 제공되는 모든 액터들은 이 함수를 사용해서 없앨수있다.
	SetLifeSpan(5.f);
}

// Called every frame
void AGreyStoneSkill1Decal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




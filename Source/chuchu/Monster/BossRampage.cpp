// Fill out your copyright notice in the Description page of Project Settings.


#include "BossRampage.h"

// Sets default values
ABossRampage::ABossRampage()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BossMeshAsset(TEXT("SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Meshes/Rampage.Rampage'"));
	if (BossMeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(BossMeshAsset.Object);
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> BossAnimAsset(TEXT("AnimBlueprint'/Game/Monster/BPRamBossAnim.BPRamBossAnim_C'"));
	if (BossAnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(BossAnimAsset.Class);
	
	m_MonsterInfoName = TEXT("BossRampage");
}

// Called when the game starts or when spawned
void ABossRampage::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABossRampage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffBlue.h"
#include "../Effect/NormalEffect.h"
#include "../Effect/MinionGunnerBullet.h"
// Sets default values
ABuffBlue::ABuffBlue()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; 

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> buffredMeshAsset(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Buff/Buff_Blue/Meshes/Buff_Blue.Buff_Blue'"));
	if (buffredMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(buffredMeshAsset.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> buffAnimAsset(TEXT("AnimBlueprint'/Game/Monster/BPBuffBlueAnim.BPBuffBlueAnim_C'"));
	if (buffAnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(buffAnimAsset.Class);

	m_MonsterInfoName = TEXT("BuffBlue");
}

// Called when the game starts or when spawned
void ABuffBlue::BeginPlay()
{	
	Super::BeginPlay();

}

// Called every frame
void ABuffBlue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuffBlue::NormalAttack()
{
	// Muzzle의 위치를 얻어온다.
	FVector	MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_01")); //소켓 위치를 얻어울수있다.

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMinionGunnerBullet* Bullet = GetWorld()->SpawnActor<AMinionGunnerBullet>(
		AMinionGunnerBullet::StaticClass(), MuzzleLoc, GetActorRotation(),
		param);

	Bullet->SetAttack(m_MonsterInfo.Attack);
	Bullet->SetOwner(this);
}
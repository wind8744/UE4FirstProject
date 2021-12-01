// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionGunner.h"
#include "../Effect/MinionGunnerBullet.h"

// Sets default values
AMinionGunner::AMinionGunner()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Ranged_Dawn.Minion_Lane_Ranged_Dawn'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/Monster/BPMinionGunnerAnim.BPMinionGunnerAnim_C'"));

	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	m_MonsterInfoName = TEXT("MinionGunner");
}

// Called when the game starts or when spawned
void AMinionGunner::BeginPlay()
{
	// 0번에 대한 다이나믹 instance가 있어야 c++에서 컨트롤이 가능(죽었을때 불타 없어지는 효과)
	//그레이스톤처럼 메테리얼이 여러개이면 조금 복잡할수도
	//부모에서 컨트롤 할 것이므로 부모 함수로 가기 전에 미리 만들어놓는다
	GetMesh()->CreateDynamicMaterialInstance(0);

	Super::BeginPlay();
}

// Called every frame
void AMinionGunner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinionGunner::NormalAttack()
{
	// Muzzle의 위치를 얻어온다.
	FVector	MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_Front")); //소켓 위치를 얻어울수있다.

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMinionGunnerBullet* Bullet = GetWorld()->SpawnActor<AMinionGunnerBullet>(
		AMinionGunnerBullet::StaticClass(), MuzzleLoc, GetActorRotation(),
		param);

	Bullet->SetAttack(m_MonsterInfo.Attack);
	Bullet->SetOwner(this);
}

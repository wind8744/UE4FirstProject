// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostTrail.h"

// Sets default values
AGhostTrail::AGhostTrail()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Mesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("Mesh"));

	SetRootComponent(m_Mesh); // 일반 메쉬랑 차이점  특정 상황에 고스트 트레일을 만들어야함 , 
	//잔상을 사용하는 메쉬가 뭐냐에[ 따라 달라질 수 있어야 하기 때문에 ㅊC++코드상으로 변경이 가능하도록  setmesh만듦

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		GhostMtrlAsset(TEXT("Material'/Game/Materials/MTGhost.MTGhost'"));

	if (GhostMtrlAsset.Succeeded())
		m_GhostMaterial = GhostMtrlAsset.Object;

	m_LifeTime = 0.2f; //살
	m_GhostType = EGhostTrailType::Fade; //fade를 많이 씀

	m_FadeTime = 0.7f; //라이프 타임 후부터 흘러가는 시간
	m_FadeTimeAcc = 0.f; //누적타임
}

// Called when the game starts or when spawned
void AGhostTrail::BeginPlay()
{
	Super::BeginPlay();

	//SetLifeSpan(1.f);
}

// Called every frame
void AGhostTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_LifeTime -= DeltaTime;

	if (m_LifeTime <= 0.f)
	{
		switch (m_GhostType)
		{
		case EGhostTrailType::LifeSpan:
			Destroy();
			break;
		case EGhostTrailType::Fade:
		{
			m_FadeTimeAcc += DeltaTime;

			float Opacity = 1.f - m_FadeTimeAcc / m_FadeTime; //투명도는 곱해지므로 시간이 더 지날수록 투명도가 커지는 

			for (auto& Mtrl : m_GhostMaterialArray)
			{
				Mtrl->SetScalarParameterValue(TEXT("Opacity"), Opacity); //투명도를 set
			}
		}
		break;
		}
	}
}

void AGhostTrail::SetMesh(const FString& Path)
{
	USkeletalMesh* Mesh = LoadObject<USkeletalMesh>(GetWorld(), *Path);

	if (Mesh)
		m_Mesh->SetSkeletalMesh(Mesh);

	int32 MtrlCount = m_Mesh->GetNumMaterials();

	//메쉬가 들어오면 메테리얼 갯수만큼 저장
	for (int32 i = 0; i < MtrlCount; ++i)
	{
		UMaterialInstanceDynamic* Mtrl = m_Mesh->CreateDynamicMaterialInstance(i, m_GhostMaterial);

		m_GhostMaterialArray.Add(Mtrl);
	}
}

void AGhostTrail::SetMesh(USkeletalMesh* Mesh)
{
	m_Mesh->SetSkeletalMesh(Mesh);

	int32 MtrlCount = m_Mesh->GetNumMaterials();

	//재질을 하나 가져와서 재질의 요소만큼 (매테리얼으 ㅣ요소가 다 다르므로)
	for (int32 i = 0; i < MtrlCount; ++i)
	{
		UMaterialInstanceDynamic* Mtrl = m_Mesh->CreateDynamicMaterialInstance(i, m_GhostMaterial);

		m_GhostMaterialArray.Add(Mtrl);
	}
}

void AGhostTrail::CopyAnimation(USkeletalMeshComponent* Com)
{
	m_Mesh->CopyPoseFromSkeletalComponent(Com); //스켈레톤 메쉬가 갖고있는 현재 공격되고 있는 애니를 복사
	//현재 스켈레톤의 최종 포즈를 갖고와서 복사해서 사용하도록 하는 것
}


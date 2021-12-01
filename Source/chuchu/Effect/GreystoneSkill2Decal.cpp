// Fill out your copyright notice in the Description page of Project Settings.


#include "GreystoneSkill2Decal.h"

/*
* 만약 어떤 물체에 데칼 안받게 하고 싶으면 디테일에 receives decals에 체크 해제하면 된다.
* 
*
*/

// Sets default values
AGreystoneSkill2Decal::AGreystoneSkill2Decal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//만든 데켈 인스턴스 불러옴
	static ConstructorHelpers::FObjectFinder<UMaterialInstance>	DecalMtrlAsset
		(TEXT("MaterialInstanceConstant'/Game/Player/GreyStone/MTGreystoneSkill2_Inst.MTGreystoneSkill2_Inst'"));

		
	if (DecalMtrlAsset.Succeeded())
		m_Decal->SetDecalMaterial(DecalMtrlAsset.Object);

	//인스턴스를 내가 만들어서 내가 거기에 여러가지의 원하는 이미지를 적용시켜서 하는것
	m_Material = m_Decal->CreateDynamicMaterialInstance(); //데칼이 동적 인스턴스를 불러옴
	//다이나믹 머테리얼 인스턴스를 불러와야 내가 c++코드에서 컨트롤이 가능하다 그 외 다른 인스턴스는 c++코드에서 컨트롤이 불가능 
	//때에 따라 이미지를 다르게 한다던가 빛나게 한다던가 등등 코드로 변경이 가능

	m_Glow = 5.f;
	m_Dir = 1.f;
}

// Called when the game starts or when spawned
void AGreystoneSkill2Decal::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGreystoneSkill2Decal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_Glow += m_Dir * DeltaTime * 5.f;

	if (m_Glow >= 10.f)
	{
		m_Glow = 5.f;
		m_Dir = -1.f;
	}

	else if (m_Glow <= 5.f)
	{
		m_Glow = 1.f;
		m_Dir = 1.f;
	}

	m_Material->SetScalarParameterValue(TEXT("Glow"), m_Glow); //내가 Glow라고 잡이놓은 파라미터 값을 찾아서 변경가능
	//m_Material->SetVectorParameterValue()
	//m_Material->Parameter

	AddActorWorldRotation(FRotator(0.f, 180.f * DeltaTime, 0.f)); //초당 180도 회전 
}

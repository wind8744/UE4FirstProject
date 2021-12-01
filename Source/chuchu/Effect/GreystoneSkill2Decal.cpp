// Fill out your copyright notice in the Description page of Project Settings.


#include "GreystoneSkill2Decal.h"

/*
* ���� � ��ü�� ��Į �ȹް� �ϰ� ������ �����Ͽ� receives decals�� üũ �����ϸ� �ȴ�.
* 
*
*/

// Sets default values
AGreystoneSkill2Decal::AGreystoneSkill2Decal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//���� ���� �ν��Ͻ� �ҷ���
	static ConstructorHelpers::FObjectFinder<UMaterialInstance>	DecalMtrlAsset
		(TEXT("MaterialInstanceConstant'/Game/Player/GreyStone/MTGreystoneSkill2_Inst.MTGreystoneSkill2_Inst'"));

		
	if (DecalMtrlAsset.Succeeded())
		m_Decal->SetDecalMaterial(DecalMtrlAsset.Object);

	//�ν��Ͻ��� ���� ���� ���� �ű⿡ ���������� ���ϴ� �̹����� ������Ѽ� �ϴ°�
	m_Material = m_Decal->CreateDynamicMaterialInstance(); //��Į�� ���� �ν��Ͻ��� �ҷ���
	//���̳��� ���׸��� �ν��Ͻ��� �ҷ��;� ���� c++�ڵ忡�� ��Ʈ���� �����ϴ� �� �� �ٸ� �ν��Ͻ��� c++�ڵ忡�� ��Ʈ���� �Ұ��� 
	//���� ���� �̹����� �ٸ��� �Ѵٴ��� ������ �Ѵٴ��� ��� �ڵ�� ������ ����

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

	m_Material->SetScalarParameterValue(TEXT("Glow"), m_Glow); //���� Glow��� ���̳��� �Ķ���� ���� ã�Ƽ� ���氡��
	//m_Material->SetVectorParameterValue()
	//m_Material->Parameter

	AddActorWorldRotation(FRotator(0.f, 180.f * DeltaTime, 0.f)); //�ʴ� 180�� ȸ�� 
}

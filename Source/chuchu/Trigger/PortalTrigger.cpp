// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalTrigger.h"
#include "../Effect/NormalEffect.h"
#include "../Skill/Meteor.h"

// Sets default values
APortalTrigger::APortalTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_PortalEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
	m_PortalEffect->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Asset(TEXT("ParticleSystem'/Game/FantasyVFXCollection/3_ParticleSystem/Portal/Portal_01/P_Portal_01_01.P_Portal_01_01'"));
	if (Asset.Succeeded())
		m_PortalEffect->SetTemplate(Asset.Object);
}

void APortalTrigger::BeginPlay()
{
	Super::BeginPlay();

	//Trigger에서 Broadcast를 하면 m_TriggerBeginDelegate에 등록된 모든 함수가 호출된다
	m_DelegateTriggerBegin.AddDynamic(this, &APortalTrigger::TriggerBegin);
	m_DelegateTriggerEnd.AddDynamic(this, &APortalTrigger::TriggerEnd);
}

// Called every frame
void APortalTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortalTrigger::TriggerBegin()
{
	//PrintViewport(1.f, FColor::Yellow, TEXT("Potal Begin !!"));
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Main2"));
}

void APortalTrigger::TriggerEnd()
{
	//PrintViewport(1.f, FColor::Red, TEXT("c End Trigger"));
}


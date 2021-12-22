// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BoxTrigger.h"
#include "PortalTrigger.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API APortalTrigger : public ABoxTrigger
{
	GENERATED_BODY()

public:
	APortalTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* m_PortalEffect;


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		void TriggerBegin();

	UFUNCTION()
		void TriggerEnd();
};

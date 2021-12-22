// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "Trigger.generated.h"

//µ®∏Æ∞‘¿Ã∆Æ (¿Œ∞≠ 55) 
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTriggerDelegate); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateTrigger);

UCLASS(BlueprintType)
class CHUCHU_API ATrigger : public AActor
{
	GENERATED_BODY()

public:
	ATrigger();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UShapeComponent* m_Trigger;

	//∆Æ∏Æ∞≈ Ω¶¿‘ ≈∏¿‘ (ƒ∏Ω∂ or π⁄Ω∫ or ...)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ETriggerType	m_ShapeType; 

	//µ®∏Æ∞‘¿Ã∆Æ 
	UPROPERTY(BlueprintAssignable, Category = "Collision")
		FDelegateTrigger	m_DelegateTriggerBegin;

	UPROPERTY(BlueprintAssignable, Category = "Collision")
		FDelegateTrigger	m_DelegateTriggerEnd;

public:
	FDelegateTrigger& GetBeginDelegate()
	{
		return m_DelegateTriggerBegin;
	}

	FDelegateTrigger& GetEndDelegate()
	{
		return m_DelegateTriggerEnd;
	}

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		void TriggerBeginOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);

	UFUNCTION()
		void TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);

};

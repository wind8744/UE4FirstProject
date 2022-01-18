// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Monster.h"
#include "Components/CapsuleComponent.h"
#include "BossRampage.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API ABossRampage : public AMonster
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossRampage();

	// 양 손 충돌체
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* m_CollisionCapsuleRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* m_CollisionCapsuleLeft;

	TArray<UMaterialInterface*> m_CurMatArray;

	UMaterialInterface* m_RedMat;

	FTimerHandle		m_OriMatTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_Atime;

	bool m_EnableAI;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NormalAttack();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	virtual void Death();
protected:
	void MakeOriMaterial();

public:
	virtual int32 GetSKillNum() //BossRampage
	{
		if (m_SkillNum >= 2) m_SkillNum = 0;
		return m_SkillNum++;
	}
};
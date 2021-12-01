// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "../Effect/GreyStoneSkill1.h"

#include "PlayerCharacter.h"
#include "Greystone.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API AGreyStone : public APlayerCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGreyStone();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> m_AttackMontageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*>	m_SkillMontageArray;
	
	UAnimMontage* m_Skill2FireMontage;

	TSubclassOf<AGreyStoneSkill1>	m_Skiill1Class; //스킬이 사용될 때 스킬bp가 생성되어야 하므로 그것을 멤버변수로 가지고 있겠다는 것
	
	class AGreystoneSkill2Decal* m_Skill2Decal;

	int32		m_AttackIndex;

	class AWeapon* m_Weapon;
	
	bool		m_OnSkill2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void Attack();
	virtual void Skill1();
	virtual void Skill2();

public:
	virtual void NormalAttack();
	virtual void AttackEnd();
	virtual void UseSkill(int32 Index);
	virtual void UseSkillFire(int32 Index);
};


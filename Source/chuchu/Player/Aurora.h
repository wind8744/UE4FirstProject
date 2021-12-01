// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PlayerCharacter.h"
#include "../Effect/AuroraSkill1.h"
#include "../Effect/GreyStoneSkill1.h"
#include "Aurora.generated.h"


UCLASS()
class CHUCHU_API AAurora : public APlayerCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAurora();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*> m_AttackMontageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*>	m_SkillMontageArray;

	UAnimMontage* m_Skill2FireMontage;
	UAnimMontage* m_DashMontage;

	TSubclassOf<AAuroraSkill1>	m_Skill1Class; //��ų�� ���� �� ��ųbp�� �����Ǿ�� �ϹǷ� �װ��� ��������� ������ �ְڴٴ� ��

	TSubclassOf<AGreyStoneSkill1>	m_Skiill1Class; //��ų�� ���� �� ��ųbp�� �����Ǿ�� �ϹǷ� �װ��� ��������� ������ �ְڴٴ� ��
	//class AGreystoneSkill2Decal* m_Skill2Decal;

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
	virtual void Skill3();
	virtual void Dash();

	void AuroraSkill2();

public:
	virtual void NormalAttack();
	virtual void AttackEnd();
	virtual void UseSkill(int32 Index);
	//virtual void UseSkillFire(int32 Index);
};


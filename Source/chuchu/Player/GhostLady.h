// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PlayerCharacter.h"
#include "../Effect/AuroraSkill1.h"
#include "../Effect/GreyStoneSkill1.h"
#include "../UIItem/ItemData.h"
#include "Components/CapsuleComponent.h"
#include "GhostLady.generated.h"
/**
 * 
 */

UCLASS()
class CHUCHU_API AGhostLady : public APlayerCharacter
{
	GENERATED_BODY()
public:
	AGhostLady();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* m_WeaponCollisionCapsule; 	//���� ĸ�� �浹ü

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*> m_AttackMontageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*>	m_SkillMontageArray;

	class AWeapon* m_Weapon;
	int32		m_AttackIndex;

	//equip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* m_Hair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* m_TopBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* m_BotBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* m_Boots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* m_Hand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* m_Helmet;

	//Sword
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* m_Sword;

	bool				m_DidEquipWeapon;

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
	virtual void Dash();
	virtual void Skill1();
	virtual void Skill2();
	virtual void Skill3();
	virtual void Skill4();

public:
	// Anim ȣ��
	virtual void NormalAttack();
	virtual void AttackEnd();
	virtual void ChangeWeaponSocket();
	virtual void UseSkill();
	
	// equip
	void InitWeaponSocket(); //���� Ż�� �� �ִϸ��̼� , ���� �ʱ�ȭ

	virtual void EquipItem(EEquipType EquipmentType, const FString& EquipmentPath); //������ ����
	virtual void RemoveItem(EEquipType EquipmentType);
	virtual void UseItem(); //������ �ȳ�

	UFUNCTION()
		void OnComponentBeginOverlapWeapon(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
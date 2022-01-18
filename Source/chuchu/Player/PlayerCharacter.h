// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "../UIItem/ItemData.h"
#include "PlayerCharacter.generated.h"

class UNiagaraSystem;

UCLASS(config = MainAsset)
class CHUCHU_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
		USpringArmComponent* m_Arm;
	
	//hp bar
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* m_HPBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneCaptureComponent2D* m_Capture;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* m_Trail;

	UAnimMontage* m_FallRecoveryMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*> m_ArrayAvoidMontage;

	class UTextureRenderTarget2D* m_FaceTarget;
	class UHPBar* m_HPBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FPlayerInfo	m_PlayerInfo;

	UPROPERTY(config) 
		FString	TestString;

	//Dash Trail
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* m_DashTrail; 
	

	//Ghost Trail
	USkeletalMesh* m_PlayerMesh; //Ghost Trail 모든 캐릭터가 쓸 수 있게

	bool		m_OnGhostTrail;
	bool		m_ActionGhostTrail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float		m_GhostTrailTime;

	float		m_GhostTrailTimeAcc;

	bool				m_MoveKey;
	bool				m_AttackEnable; //공격 가능상태인지 아닌지
	bool				m_Attacking;    //공격중인지 아닌지
	bool				m_Death;

	class UPlayerAnim* m_AnimInstance;

	//Dash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_DashDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_DashCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_DashStop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_TargetFov;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_FovSpeed;

	int m_DashFov;
	bool m_CanDash;
	int32 m_DashMontageIdx; 
	int32 m_PushedSkillIdx;

	FTimerHandle UnusedHandle;

	//Avoid
	float m_SideValue;
	float m_FrontValue;

	virtual void EquipItem(EEquipType EquipmentType, const FString& EquipmentPath); //아이템 장착
	virtual void RemoveItem(EEquipType EquipmentType); //아이템 해제
	virtual void UseItem(); //아이템 냠냠


	//test
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_test;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_test2;


public:
	const FPlayerInfo& GetPlayerInfo()	const
	{
		return m_PlayerInfo;
	}
	void SetPlayerInfoHP(const int32 Hp)
	{
		m_PlayerInfo.HP = Hp;
	}
	void SetPlayerInfoName(const FString& Name)
	{
		m_PlayerInfo.Name = Name;
	}
	bool IsMoveKey()	const
	{
		return m_MoveKey;
	}
	void EnableAttack(bool Enable)
	{
		m_AttackEnable = Enable;
	}

	FVector GetCameraLocation()	const
	{
		return m_Camera->GetComponentLocation();
	}
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveFrontKey(float Scale);
	void MoveSideKey(float Scale);
	void RotationZKey(float Scale);
	void CameraZoomKey(float Scale);
	void CameraLookUpKey(float Scale);
	void CameraLookLRKey(float Scale);
	void SpeedUpKey(float Scale);
	
	void PlayFallRecovery();
	void JumpKey(); //액션키는 input이 없다.
	void AttackKey();
	void Skill1Key();
	void Skill2Key();
	void Skill3Key();
	void Skill3KeyReleased();
	void Skill4Key();
	void DashKey();
	void EquipWeaponKey();
	void DrinkKey();

protected:
	virtual void Attack();
	virtual void Skill1();
	virtual void Skill2();
	virtual void Skill3();
	virtual void Skill3Released();
	virtual void Skill4();
	virtual void Dash();

public:
	virtual void NormalAttack();
	virtual void AttackEnd();
	virtual void UseSkill();
	virtual void UseSkill(int32 Index);
	virtual void UseSkillFire(int32 Index);
	virtual void Skill3Loop();
	
	void GhostTrailEnd();
	void OnGhostTrail();

	void StopDashing();
	void ResetDash();

	void ShakeCamera();
	void TimeDilation(float _Time);

	virtual void ChangeWeaponSocket();
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	
public:
	UFUNCTION()
		void NameWidgetCallback();

	UFUNCTION()
		void CharacterHUDNameWidgetCallback();

public:
	void FootStep(bool Left);
	void AddGold(int32 _gold);
	void AddExp(int32 _exp);

};
// Fill out your copyright notice in the Description page of Project Settings.

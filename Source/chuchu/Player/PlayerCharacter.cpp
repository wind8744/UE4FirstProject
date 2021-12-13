// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "PlayerAnim.h"
#include "../Effect/HitCameraShake.h"
#include "../chuchuGameModeBase.h"
#include "../UI/MainHUD.h"
#include "../UI/CharacterHUD.h"
#include "../UI/HPBar.h"
#include "../chuchuGameInstance.h"
#include "../Material/PhysicalMaterialBase.h"
#include "../Effect/GhostTrail.h"

/*
�𸮾���4���� �����ڴ� �����Ϳ��� ������ ������ ��� �����Ϳ� ��ġ�Ǵ� �� ���� ȣ��ȴ�.
�����Ϳ��� ������ �÷����Ҷ��� �����ڰ� ȣ���� �ȵȴ�.

��Ű¡�� ���� ���� �������Ϸ� ������ �Ѵٸ� �����ڰ� ���ӳ��� ��ġ�ɶ� ȣ��ȴ�.
*/
// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	
	//Arm �� �θ� Component�� Mesh�� ����
	m_Arm->SetupAttachment(GetMesh());
	m_Arm->TargetArmLength = 900.f;

	//Camera �� �θ� Component�� Mesh�� �����Ѵ�]
	m_Camera->SetupAttachment(m_Arm);
	
	m_Capture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture"));
	m_Capture->SetupAttachment(GetMesh());


	m_Trail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));
	m_Trail->SetupAttachment(GetMesh()); //�޽��� ���� �޾���

	GetCharacterMovement()->JumpZVelocity = 600.f;

	m_MoveKey = false;
	m_AttackEnable = true;

	//�������� ����ϰٵ��� ����
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	//��Į�� �ȹ޾ƾ� �� ��ü�� �ִ� 
	//�׷����� ��ü �����Ͽ��� receive�� �̻��ϸ�
	//	�������� receives decals�� üũ ������ ���ָ� ��Į�� ������ ���� �ʴ´�
	GetMesh()->SetReceivesDecals(false);
	SetCanBeDamaged(true);

	//UI asset �������Ʈ ��������
	//UI hpbar
	m_HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	m_HPBar->SetupAttachment(GetMesh());

	/*
	static ConstructorHelpers::FClassFinder<UUserWidget>	HPBarAsset(TEXT("WidgetBlueprint'/Game/UI/UI_HPBar.UI_HPBar_C'"));

	if (HPBarAsset.Succeeded())
		m_HPBar->SetWidgetClass(HPBarAsset.Class);

	m_HPBar->SetWidgetSpace(EWidgetSpace::Screen); // ��ü�� �������� �ʴ´� world�� �޽��� ������
	m_HPBar->SetDrawSize(FVector2D(200.f, 60.f));
	m_HPBar->SetRelativeLocation(FVector(0.f, 0.f, 230.f));
	m_HPBar->SetBlendMode(EWidgetBlendMode::Transparent);
	*/

	//���� Ÿ�� �ؽ���( �� ������ ��)
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D>	FaceTargetObj(TEXT("TextureRenderTarget2D'/Game/Player/RTPlayerFace.RTPlayerFace'"));

	if (FaceTargetObj.Succeeded())
		m_FaceTarget = FaceTargetObj.Object;

	m_Capture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList; //�󱼸� ������(��� ������� üũ)
	m_Capture->CaptureSource = ESceneCaptureSource::SCS_SceneColorSceneDepth;

	m_Capture->TextureTarget = m_FaceTarget;

	m_Capture->SetRelativeLocation(FVector(-0.000008f, 59.998909f, 170.f));
	m_Capture->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	m_Death = false;

	m_FallRecoveryMontage = nullptr;
	//m_AvoidMontage = nullptr;

	//��Ʈ Ʈ����
	m_PlayerMesh = nullptr;

	m_OnGhostTrail = false;
	m_ActionGhostTrail = false;
	m_GhostTrailTime = 0.2f;
	m_GhostTrailTimeAcc = 0.f;

	//Dash
	m_CanDash = true;
	m_DashDistance = 3000.f;
	m_DashCooldown = 1.f; //�뽬 ��Ÿ��
	m_DashStop = 0.3f;
	m_DashFov = 0;		  //�뽬 ���� 1 �뽬 �� 2 (fov������ ����)
	m_TargetFov = 120.f;  //�뽬 �� ���� target fov��
	m_FovSpeed = 2.f;		  //�뽬 �� ���� fov speed ��

	m_PushedSkillIdx = 0;
	//Inventory
	//Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	//	Inventory->Capacity = 20;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Hp bar widget
	/*
		m_HPBarWidget = Cast<UHPBar>(m_HPBar->GetWidget()); //??? user interface -> tickmode disable�� �����ϸ� �ȶ�.. �Ѥ�?
		m_HPBarWidget->SetDelegate<APlayerCharacter>(this, &APlayerCharacter::NameWidgetCallback);
	*/
	m_AnimInstance = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());

	AchuchuGameModeBase* GameMode = Cast<AchuchuGameModeBase>(GetWorld()->GetAuthGameMode());

	if (IsValid(GameMode))
	{
		UMainHUD* MainHUD = GameMode->GetMainHUD();

		if (IsValid(MainHUD))
		{
			UCharacterHUD* CharacterHUD = MainHUD->GetCharacterHUD();

			if (IsValid(CharacterHUD))
			{
				CharacterHUD->SetDelegate<APlayerCharacter>(this, &APlayerCharacter::CharacterHUDNameWidgetCallback);
			}
		}
	}
	//m_HPBarWidget->SetName(m_PlayerInfo.Name);
	m_Capture->ShowOnlyActors.Add(this); //ī�޶󿡼� ���� �� ����Ÿ�ٿ� ����

	LOGSTRING(TestString);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
#if ENABLE_DRAW_DEBUG
	EPlayerAnimType a = m_AnimInstance->GetAnimType();
	if (a != EPlayerAnimType::Ground)
	{
		FString t = GetEnumToString(a);
		LOGSTRING(GetEnumToString(a));
		//PrintViewport(1.5f, FColor::Red, t);
	}
#endif
	
	// =================
	// �뽬 - ī�޶� ȿ��
	// =================
	if (1 == m_DashFov) 
	{
		//ī�޶�
		float CameraFov = m_Camera->FieldOfView;
		float alpha = DeltaTime * m_FovSpeed;
		float RetValue = FMath::Lerp(CameraFov, m_TargetFov, alpha);

		m_Camera->FieldOfView = RetValue;
	}
	else if (2 == m_DashFov)
	{
		//ī�޶�
		float CameraFov = m_Camera->FieldOfView;
		float alpha = DeltaTime * m_FovSpeed;
		float inputfov = 88.f;
		float RetValue = FMath::Lerp(CameraFov, inputfov, alpha);

		m_Camera->FieldOfView = RetValue;
	}

	// =================
	// Trail effect
	// =================
	if (GetCharacterMovement()->Velocity.Size() > 0.f) 	//Trail�� ȸ���� �� ������ �ʵ��� !! 
	{
		m_Trail->SetEmitterEnable(TEXT("Trail"), true);
	}
	else
	{
		m_Trail->SetEmitterEnable(TEXT("Trail"), false);
	}

	//��Ʈ Ʈ���� ����
	if (m_OnGhostTrail || m_ActionGhostTrail)
	{
		m_GhostTrailTimeAcc += DeltaTime;

		if (m_GhostTrailTimeAcc >= m_GhostTrailTime)
		{
			m_GhostTrailTimeAcc -= m_GhostTrailTime;

			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Ghost Trail ����
			AGhostTrail* Trail = GetWorld()->SpawnActor<AGhostTrail>(AGhostTrail::StaticClass(),
				GetMesh()->GetComponentLocation(),
				GetMesh()->GetComponentRotation(), param);

			Trail->SetMesh(m_PlayerMesh);
			Trail->CopyAnimation(GetMesh()); //���̷����� ���� ������� ��� ������ �´�
		}
	}
	
	// ============
	// InputMove
	// ============
	if (m_Attacking)
	{
		AddMovementInput(FVector(m_Camera->GetForwardVector().X, m_Camera->GetForwardVector().Y, 0.f).GetSafeNormal(), 0.2,true);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	
	PlayerInputComponent->BindAxis(TEXT("RotationZ"), this, &APlayerCharacter::RotationZKey);
	PlayerInputComponent->BindAxis(TEXT("CameraZoom"), this, &APlayerCharacter::CameraZoomKey);
	PlayerInputComponent->BindAxis(TEXT("CameraLookUp"), this, &APlayerCharacter::CameraLookUpKey);
	PlayerInputComponent->BindAxis(TEXT("CameraLookLR"), this, &APlayerCharacter::CameraLookLRKey);

	PlayerInputComponent->BindAxis(TEXT("MoveFront"), this, &APlayerCharacter::MoveFrontKey);
	PlayerInputComponent->BindAxis(TEXT("MoveSide"), this, &APlayerCharacter::MoveSideKey);
	PlayerInputComponent->BindAxis(TEXT("SpeedUp"), this, &APlayerCharacter::SpeedUpKey);

	//�׼�Ű
	//pressed : ������
	//Released : Ű�� �����ٰ� �� ��
	PlayerInputComponent->BindAction(TEXT("Jump"),EInputEvent::IE_Pressed,this, &APlayerCharacter::JumpKey);
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &APlayerCharacter::DashKey);
	PlayerInputComponent->BindAction(TEXT("EquipWeapon"), EInputEvent::IE_Pressed, this, &APlayerCharacter::EquipWeaponKey);//
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::AttackKey);
	PlayerInputComponent->BindAction(TEXT("Skill1"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Skill1Key);
	PlayerInputComponent->BindAction(TEXT("Skill2"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Skill2Key);
	PlayerInputComponent->BindAction(TEXT("Skill3"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Skill3Key);
	PlayerInputComponent->BindAction(TEXT("Skill3"), EInputEvent::IE_Released, this, &APlayerCharacter::Skill3KeyReleased);
	PlayerInputComponent->BindAction(TEXT("Skill4"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Skill4Key);
}

void APlayerCharacter::Skill3KeyReleased()
{
	Skill3Released();
}

void APlayerCharacter::SpeedUpKey(float Scale)
{
	m_AnimInstance->SetSpeed(Scale);
}
void APlayerCharacter::MoveFrontKey(float Scale) //Ű�� ������ �� ������ ���� ���޵ȴ�. w�� 1 s�� -1 �Ѵٴ����� 0
{
	m_FrontValue = Scale;

	if (!m_AttackEnable || m_AnimInstance->GetAnimType() == EPlayerAnimType::Skill) return; // �����̸鼭 ���� ����

	AddMovementInput(GetActorForwardVector(), Scale);

	//������
	if (Scale == 0.f)
	{
		m_DashMontageIdx = 0;
		m_MoveKey = false;
		m_AnimInstance->SetDirection(0.f);
	}
		
	//��
	else if (Scale == 1.f)
	{
		m_DashMontageIdx = 0;
		m_MoveKey = true;
		m_AnimInstance->SetDirection(0.f);
	}
		
	//��
	else
	{
		m_DashMontageIdx = 1;
		m_MoveKey = true;
		m_AnimInstance->SetDirection(180.f);
	}
		
}
void APlayerCharacter::MoveSideKey(float Scale) // ���� ������ ������
{
	m_SideValue = Scale;

	if (!m_AttackEnable || m_AnimInstance->GetAnimType() == EPlayerAnimType::Skill) return; // �����̸鼭 ���� ����

	AddMovementInput(GetActorRightVector(), Scale);
	
	float Direction = m_AnimInstance->GetDirection();
	if (Scale == 1.f)
	{
		m_DashMontageIdx = 3; 

		m_MoveKey = true;
		if (Direction == 0.f)
			m_AnimInstance->SetDirection(45.f);
		else
			m_AnimInstance->SetDirection(135.f);
	}
	else if (Scale == -1.f)
	{
		m_DashMontageIdx = 2;

		m_MoveKey = true;
		if (Direction == 0.f)
			m_AnimInstance->SetDirection(-45.f);
		else
			m_AnimInstance->SetDirection(-135.f);
	}

	// Ghost Trail
	/*
	if (m_MoveKey)
	{
		if (!m_OnGhostTrail)
		{
			m_GhostTrailTimeAcc = 0.f;
			m_ActionGhostTrail = false;
		}
		m_OnGhostTrail = true;
	}
	else
	{
		m_OnGhostTrail = false;
	}
	*/
}
void APlayerCharacter::RotationZKey(float Scale)
{
	AddControllerYawInput(Scale);
}	
void APlayerCharacter::CameraZoomKey(float Scale)
{
	// ����������� arm�� ���� ����
	m_Arm->TargetArmLength -= Scale * 20.f;
	if (m_Arm->TargetArmLength < 30.f)
	{
		m_Arm->TargetArmLength = 30.f;
	}
	else if (m_Arm->TargetArmLength > 900.f)
	{
		m_Arm->TargetArmLength = 900.f;
	}
}
void APlayerCharacter::CameraLookUpKey(float Scale)
{
	//4���� ȸ�� ���� -> ã�ƺ��� 
	FRotator Rot = m_Arm->GetRelativeRotation();
	Rot.Pitch += Scale * 100.f * GetWorld()->GetDeltaSeconds();

	if (Rot.Pitch > 40.f)
		Rot.Pitch = 40.f;

	else if (Rot.Pitch < -45.f)
		Rot.Pitch = -45.f;
	m_Arm->SetRelativeRotation(Rot);
} 

void APlayerCharacter::CameraLookLRKey(float Scale)
{
	FRotator Rot = m_Arm->GetRelativeRotation();
	Rot.Yaw += Scale * 100.f * GetWorld()->GetDeltaSeconds();
	m_Arm->SetRelativeRotation(Rot);
}

void APlayerCharacter::JumpKey()
{
	if (m_Death)
		return;

	if (m_AnimInstance->GetAnimType() == EPlayerAnimType::Ground)
	{
		Jump(); //�𸮾󿡼� ���� ����� ������ 
		m_AnimInstance->ChangeAnimType(EPlayerAnimType::Jump);
	}
}
void APlayerCharacter::AttackKey()
{
	if (m_AnimInstance->GetAnimType() == EPlayerAnimType::Skill) return; // ��ų �� �� ���� ����

	if (m_AttackEnable)
	{
		m_AttackEnable = false; //���� Ű�� �ѹ� ������ ���� ���� ���°� �ƴϰ� �ȴ� �ߺ� ���°� 
		m_Attacking = true; //�������ΰ�?
		m_AnimInstance->SetAttackEnable(true);
		Attack();
	}
}

void APlayerCharacter::EquipWeaponKey()
{
	if (m_Death)
		return;

	if (m_AnimInstance->GetAnimType() == EPlayerAnimType::Ground)
	{
		m_AnimInstance->ChangeAnimType(EPlayerAnimType::Equip);
	}
	
}

void APlayerCharacter::DashKey()
{
	if (m_AnimInstance->GetAnimType() != EPlayerAnimType::Ground) return;

	//* Anim BP���� ��� ����� ��Ÿ�� ����� �����ϵ��� �Ǿ��־, ��� �������� �ʾ��� �� ~ ��û�� �� ��Ƽ���̰� �Ҹ��� �ʴ´�. ~
	//int32 MontageIdx = 0;
	if (m_CanDash && m_AttackEnable) // �׷��Ƿ� ���� ���ɽÿ��� ��ø� ����� �� �ֵ��� 
	{
		Dash(); //ĳ���� fx

		if (m_FrontValue == 0 && m_SideValue == 0) //�⺻ �ձ�����
		{
			GetCharacterMovement()->BrakingFrictionFactor = 0.f; //�������� 0���� ����, ���� ������ �ӵ��� �������� ���� �����ϱ� ����
			LaunchCharacter(FVector(m_Camera->GetForwardVector().X, m_Camera->GetForwardVector().Y, 0).GetSafeNormal() * m_DashDistance, true, true);//ī�޶� ���Ʒ��� ������ ����, ������ ������ �������� ĳ���� �߻�
		}
		else
		{
			/*
			FVector GetVel = FVector(m_FrontValue, m_SideValue, 0.f); // ������ �ٲ�� ����� ������ 
			GetVel.Normalize();
			*/
			FVector GetVel = GetVelocity(); // ������ Actor �ӵ��� ���� �� �ִ�.
			GetVel.Normalize();
			GetCharacterMovement()->BrakingFrictionFactor = 0.f; //�������� 0���� ����, ���� ������ �ӵ��� �������� ���� �����ϱ� ����
			LaunchCharacter(GetVel * m_DashDistance, true, true);
		}
		//��Ÿ�� ���
		m_AnimInstance->ChangeAnimType(EPlayerAnimType::Avoid);
		PlayAnimMontage(m_ArrayAvoidMontage[m_DashMontageIdx], 1.8, NAME_None);
		
		m_CanDash = false;
		m_DashFov = 1;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayerCharacter::StopDashing, m_DashStop, false); //dashstop�ð� ���� �Լ� ȣ��
	}
	
}

void APlayerCharacter::StopDashing()
{
	m_DashFov = 2;
	//GetCharacterMovement()->StopMovementImmediately(); //�̵� ���� ȣ��
	GetCharacterMovement()->BrakingFrictionFactor = 2.f; //������ �ʱ�ȭ
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayerCharacter::ResetDash, m_DashCooldown, false); //dashstop�ð� ���� �Լ� ȣ��
}

void APlayerCharacter::ResetDash()
{
	m_CanDash = true;
	m_DashFov = 0;
	m_Camera->FieldOfView = 90.f;
}

void APlayerCharacter::Skill1Key()
{
	m_PushedSkillIdx = 1;
	Skill1(); // playercharacter�� ��ӹ��� Ŭ������ skill1 �Լ��� ȣ�� (greystone class�� �Լ��� ȣ��)
}
void APlayerCharacter::Skill2Key()
{
	m_PushedSkillIdx = 2;
	Skill2(); // playercharacter�� ��ӹ��� Ŭ������ skill2 �Լ��� ȣ�� (greystone class�� �Լ��� ȣ��)
}
void APlayerCharacter::Skill3Key()
{
	m_PushedSkillIdx = 3;
	Skill3();
}
void APlayerCharacter::Skill4Key()
{
	m_PushedSkillIdx = 4;
	Skill4(); 
}

void APlayerCharacter::GhostTrailEnd()
{
	m_OnGhostTrail = false;
	m_ActionGhostTrail = false;
	m_GhostTrailTimeAcc = 0.f;
}

void APlayerCharacter::OnGhostTrail()
{
	m_OnGhostTrail = true;
	m_ActionGhostTrail = true;
	m_GhostTrailTimeAcc = 0.f;
}

float APlayerCharacter::TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent,class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Damage == 0.f)
		return 0.f;

	Damage = Damage - m_PlayerInfo.Armor;
	Damage = Damage < 1.f ? 1.f : Damage;

	m_PlayerInfo.HP -= Damage;

	// �÷��̾ �׾��� ���
	if (m_PlayerInfo.HP <= 0)
	{
		//m_AnimInstance->ChangeAnimType(EMonsterAnimType::Death);

		//AMonsterAIController* MonsterController = Cast<AMonsterAIController>(GetController());

		//if (MonsterController)
		//	MonsterController->BrainComponent->StopLogic(TEXT("Dead"));
	}
	
	// UI
	AchuchuGameModeBase* GameMode = Cast<AchuchuGameModeBase>(GetWorld()->GetAuthGameMode());

	//ĳ���� hp
	if (IsValid(GameMode))
	{
		UMainHUD* MainHUD = GameMode->GetMainHUD();

		if (IsValid(MainHUD))
		{
			UCharacterHUD* CharacterHUD = MainHUD->GetCharacterHUD();

			if (IsValid(CharacterHUD))
			{
				CharacterHUD->SetHPPercent(m_PlayerInfo.HP / (float)m_PlayerInfo.HPMax);
			}
		}
	}
	/*
	if (IsValid(m_HPBarWidget)) 
	{
		m_HPBarWidget->SetHPPercent(m_PlayerInfo.HP / (float)m_PlayerInfo.HPMax);
	}
	*/

	//ī�޶� ����Ʈ
	//GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(UHitCameraShake::StaticClass());
	ShakeCamera();

	return Damage;
}

void APlayerCharacter::NameWidgetCallback()
{
	//�ν��Ͻ��� �̸� �����س����� �ҷ��ͼ� ui�� set
	UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());
	const FString playername = GameInst->GetPlayerInfoName();
	m_HPBarWidget->SetName(playername);
}

void APlayerCharacter::CharacterHUDNameWidgetCallback()
{
	AchuchuGameModeBase* GameMode = Cast<AchuchuGameModeBase>(GetWorld()->GetAuthGameMode());

	if (IsValid(GameMode))
	{
		UMainHUD* MainHUD = GameMode->GetMainHUD();

		if (IsValid(MainHUD))
		{
			UCharacterHUD* CharacterHUD = MainHUD->GetCharacterHUD();

			if (IsValid(CharacterHUD))
			{
				//��
				UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());
				const FString playername = GameInst->GetPlayerInfoName();
				CharacterHUD->SetName(playername);
			}
		}
	}
}

void APlayerCharacter::ShakeCamera()
{
	//ī�޶� ����Ʈ
	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(UHitCameraShake::StaticClass());
}

void APlayerCharacter::TimeDilation(float _Time)
{
	GetWorldSettings()->SetTimeDilation(_Time);
}


void APlayerCharacter::PlayFallRecovery()
{
	if (!m_FallRecoveryMontage)
		return;

	if (!m_AnimInstance->Montage_IsPlaying(m_FallRecoveryMontage))
	{
		m_AnimInstance->Montage_SetPosition(m_FallRecoveryMontage, 0.f);

		m_AnimInstance->Montage_Play(m_FallRecoveryMontage);
	}
}

void APlayerCharacter::FootStep(bool Left)
{
	// ������ ��ġ�� ���´�.
	FVector	SocketLoc;

	//���̷��濡�� �����̸����� ���� ������
	if (Left)
		SocketLoc = GetMesh()->GetSocketLocation(TEXT("Foot_L"));

	else
		SocketLoc = GetMesh()->GetSocketLocation(TEXT("Foot_R"));

	// ������ ��ġ���� �Ʒ��� �����浹�� �Ѵ�.
	FCollisionQueryParams	params(NAME_None, false, this);
	params.bReturnPhysicalMaterial = true; //�����ø��׸����� ����ϰٴ�

	FHitResult	result; //�浹ó��
	bool Hit = GetWorld()->LineTraceSingleByChannel(result, SocketLoc, SocketLoc + FVector(0.f, 0.f, -100.f), //-1m �Ʒ�
		ECollisionChannel::ECC_GameTraceChannel5, params); //���彺������ �浹ü
	//bool Hit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), ); //������

	if (Hit)
	{
		UPhysicalMaterialBase* Phys = Cast<UPhysicalMaterialBase>(result.PhysMaterial); //�ε��� �༮�� ���������׸����� �����

		if (Phys)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Phys->GetSound(), result.ImpactPoint);
		}
	}
}

void APlayerCharacter::Skill1() {}
void APlayerCharacter::Skill2() {}
void APlayerCharacter::Skill3() {}
void APlayerCharacter::Skill3Released(){}
void APlayerCharacter::Skill4() {}
void APlayerCharacter::Dash() {}
void APlayerCharacter::Attack() {}
void APlayerCharacter::NormalAttack() {}
void APlayerCharacter::AttackEnd()
{
	m_AttackEnable = true;
	m_Attacking = false; //�������ΰ�?
}
void APlayerCharacter::UseSkill() {}
void APlayerCharacter::UseSkill(int32 Index) {}
void APlayerCharacter::UseSkillFire(int32 Index) {}
void APlayerCharacter::RemoveItem(EEquipType EquipmentType) {}
void APlayerCharacter::EquipItem(EEquipType EquipmentType, const FString& EquipmentPath) {}
void APlayerCharacter::UseItem() {}
void APlayerCharacter::ChangeWeaponSocket() {}
void APlayerCharacter::Skill3Loop(){}

void APlayerCharacter::AddGold(int32 _gold)
{
	m_PlayerInfo.Gold += _gold;
	AchuchuGameModeBase* GameMode = Cast<AchuchuGameModeBase>(GetWorld()->GetAuthGameMode());
	//if (GameMode)
	//	GameMode->GetMainHUD()->GetInventory()->SetGold(m_PlayerInfo.Gold);


}
void APlayerCharacter::AddExp(int32 _exp)
{
	m_PlayerInfo.Exp += _exp;

	AchuchuGameModeBase* GameMode = Cast<AchuchuGameModeBase>(GetWorld()->GetAuthGameMode());
	//if (GameMode)
	//	GameMode->GetMainHUD()->GetInventory()->SetGold(m_PlayerInfo.Gold);
}
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
언리얼엔진4에서 생성자는 에디터에서 게임을 실행할 경우 에디터에 배치되는 그 순간 호출된다.
에디터에서 게임을 플레이할때는 생성자가 호출이 안된다.

패키징된 최종 게임 실행파일로 실행을 한다면 생성자가 게임내에 배치될때 호출된다.
*/
// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	
	//Arm 의 부모 Component로 Mesh를 지정
	m_Arm->SetupAttachment(GetMesh());
	m_Arm->TargetArmLength = 900.f;

	//Camera 의 부모 Component로 Mesh를 지정한다]
	m_Camera->SetupAttachment(m_Arm);
	
	m_Capture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture"));
	m_Capture->SetupAttachment(GetMesh());


	m_Trail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));
	m_Trail->SetupAttachment(GetMesh()); //메쉬에 꼬리 달아줌

	GetCharacterMovement()->JumpZVelocity = 600.f;

	m_MoveKey = false;
	m_AttackEnable = true;

	//프로파일 사용하겟따고 지정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	//데칼을 안받아야 할 물체도 있다 
	//그런것은 물체 디테일에서 receive를 겁색하면
	//	렌더링의 receives decals에 체크 해제를 해주면 데칼의 영향을 받지 않는다
	GetMesh()->SetReceivesDecals(false);
	SetCanBeDamaged(true);

	//UI asset 블루프린트 가져오기
	//UI hpbar
	m_HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	m_HPBar->SetupAttachment(GetMesh());

	/*
	static ConstructorHelpers::FClassFinder<UUserWidget>	HPBarAsset(TEXT("WidgetBlueprint'/Game/UI/UI_HPBar.UI_HPBar_C'"));

	if (HPBarAsset.Succeeded())
		m_HPBar->SetWidgetClass(HPBarAsset.Class);

	m_HPBar->SetWidgetSpace(EWidgetSpace::Screen); // 물체에 가려지지 않는다 world는 메쉬에 가려짐
	m_HPBar->SetDrawSize(FVector2D(200.f, 60.f));
	m_HPBar->SetRelativeLocation(FVector(0.f, 0.f, 230.f));
	m_HPBar->SetBlendMode(EWidgetBlendMode::Transparent);
	*/

	//렌더 타겟 텍스쳐( 얼굴 찍히는 것)
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D>	FaceTargetObj(TEXT("TextureRenderTarget2D'/Game/Player/RTPlayerFace.RTPlayerFace'"));

	if (FaceTargetObj.Succeeded())
		m_FaceTarget = FaceTargetObj.Object;

	m_Capture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList; //얼굴만 찍히게(배경 사라지게 체크)
	m_Capture->CaptureSource = ESceneCaptureSource::SCS_SceneColorSceneDepth;

	m_Capture->TextureTarget = m_FaceTarget;

	m_Capture->SetRelativeLocation(FVector(-0.000008f, 59.998909f, 170.f));
	m_Capture->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	m_Death = false;

	m_FallRecoveryMontage = nullptr;
	//m_AvoidMontage = nullptr;

	//고스트 트레일
	m_PlayerMesh = nullptr;

	m_OnGhostTrail = false;
	m_ActionGhostTrail = false;
	m_GhostTrailTime = 0.2f;
	m_GhostTrailTimeAcc = 0.f;

	//Dash
	m_CanDash = true;
	m_DashDistance = 3000.f;
	m_DashCooldown = 1.f; //대쉬 쿨타임
	m_DashStop = 0.3f;
	m_DashFov = 0;		  //대쉬 시작 1 대쉬 끝 2 (fov보간을 위한)
	m_TargetFov = 120.f;  //대쉬 할 때의 target fov값
	m_FovSpeed = 2.f;		  //대쉬 할 때의 fov speed 값

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
		m_HPBarWidget = Cast<UHPBar>(m_HPBar->GetWidget()); //??? user interface -> tickmode disable로 설정하면 안뜸.. ㅡㅡ?
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
	m_Capture->ShowOnlyActors.Add(this); //카메라에서 사진 찍어서 렌더타겟에 저장

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
	// 대쉬 - 카메라 효과
	// =================
	if (1 == m_DashFov) 
	{
		float CameraFov = m_Camera->FieldOfView;
		float alpha = DeltaTime * m_FovSpeed;
		float RetValue = FMath::Lerp(CameraFov, m_TargetFov, alpha);

		m_Camera->FieldOfView = RetValue;
	}
	else if (2 == m_DashFov)
	{
		float CameraFov = m_Camera->FieldOfView;
		float alpha = DeltaTime * m_FovSpeed;
		float inputfov = 88.f;
		float RetValue = FMath::Lerp(CameraFov, inputfov, alpha);

		m_Camera->FieldOfView = RetValue;
	}

	// =================
	// Trail effect
	// =================
	if (GetCharacterMovement()->Velocity.Size() > 0.f) 	//Trail이 회전할 때 나오지 않도록 !! 
	{
		m_Trail->SetEmitterEnable(TEXT("Trail"), true);
	}
	else
	{
		m_Trail->SetEmitterEnable(TEXT("Trail"), false);
	}

	//고스트 트레일 생성
	if (m_OnGhostTrail || m_ActionGhostTrail)
	{
		m_GhostTrailTimeAcc += DeltaTime;

		if (m_GhostTrailTimeAcc >= m_GhostTrailTime)
		{
			m_GhostTrailTimeAcc -= m_GhostTrailTime;

			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Ghost Trail 생성
			AGhostTrail* Trail = GetWorld()->SpawnActor<AGhostTrail>(AGhostTrail::StaticClass(),
				GetMesh()->GetComponentLocation(),
				GetMesh()->GetComponentRotation(), param);

			Trail->SetMesh(m_PlayerMesh);
			Trail->CopyAnimation(GetMesh()); //스켈레톤의 현재 재생중인 포즈를 가지고 온다
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

	//액션키
	//pressed : 누를때
	//Released : 키를 눌렀다가 뗄 때
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
void APlayerCharacter::MoveFrontKey(float Scale) //키를 누를때 각 스케일 값이 전달된다. w는 1 s는 -1 둘다누르면 0
{
	m_FrontValue = Scale;

	if (!m_AttackEnable || m_AnimInstance->GetAnimType() == EPlayerAnimType::Skill) return; // 움직이면서 공격 막음

	AddMovementInput(GetActorForwardVector(), Scale);

	//가만히
	if (Scale == 0.f)
	{
		m_DashMontageIdx = 0;
		m_MoveKey = false;
		m_AnimInstance->SetDirection(0.f);
	}
		
	//앞
	else if (Scale == 1.f)
	{
		m_DashMontageIdx = 0;
		m_MoveKey = true;
		m_AnimInstance->SetDirection(0.f);
	}
		
	//뒤
	else
	{
		m_DashMontageIdx = 1;
		m_MoveKey = true;
		m_AnimInstance->SetDirection(180.f);
	}
		
}
void APlayerCharacter::MoveSideKey(float Scale) // 왼쪽 오른쪽 값들어옴
{
	m_SideValue = Scale;

	if (!m_AttackEnable || m_AnimInstance->GetAnimType() == EPlayerAnimType::Skill) return; // 움직이면서 공격 막음

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
	// 블루프린터의 arm의 변수 조정
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
	//4원소 회전 지원 -> 찾아보기 
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
		Jump(); //언리얼에서 점프 기능을 제공함 
		m_AnimInstance->ChangeAnimType(EPlayerAnimType::Jump);
	}
}
void APlayerCharacter::AttackKey()
{
	if (m_AnimInstance->GetAnimType() == EPlayerAnimType::Skill) return; // 스킬 쓸 때 공격 막음

	if (m_AttackEnable)
	{
		m_AttackEnable = false; //어택 키가 한번 들어오는 순간 공격 상태가 아니게 된다 중복 막는것 
		m_Attacking = true; //공격중인가?
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

	//* Anim BP에서 장비 착용시 몽타주 재생이 가능하도록 되어있어서, 장비 착용하지 않았을 때 ~ 대시사용 시 노티파이가 불리지 않는다. ~
	//int32 MontageIdx = 0;
	if (m_CanDash && m_AttackEnable) // 그러므로 공격 가능시에만 대시를 사용할 수 있도록 
	{
		Dash(); //캐릭터 fx

		if (m_FrontValue == 0 && m_SideValue == 0) //기본 앞구르기
		{
			GetCharacterMovement()->BrakingFrictionFactor = 0.f; //마찰력을 0으로 설정, 땅에 닿을때 속도가 느려지는 것을 방지하기 위함
			LaunchCharacter(FVector(m_Camera->GetForwardVector().X, m_Camera->GetForwardVector().Y, 0).GetSafeNormal() * m_DashDistance, true, true);//카메라 위아래로 움직짐 방지, 지정된 단위의 방향으로 캐릭터 발사
		}
		else
		{
			/*
			FVector GetVel = FVector(m_FrontValue, m_SideValue, 0.f); // 시점이 바뀌면 딴대로 굴러감 
			GetVel.Normalize();
			*/
			FVector GetVel = GetVelocity(); // 현재의 Actor 속도를 구할 수 있다.
			GetVel.Normalize();
			GetCharacterMovement()->BrakingFrictionFactor = 0.f; //마찰력을 0으로 설정, 땅에 닿을때 속도가 느려지는 것을 방지하기 위함
			LaunchCharacter(GetVel * m_DashDistance, true, true);
		}
		//몽타주 재생
		m_AnimInstance->ChangeAnimType(EPlayerAnimType::Avoid);
		PlayAnimMontage(m_ArrayAvoidMontage[m_DashMontageIdx], 1.8, NAME_None);
		
		m_CanDash = false;
		m_DashFov = 1;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayerCharacter::StopDashing, m_DashStop, false); //dashstop시간 이후 함수 호출
	}
	
}

void APlayerCharacter::StopDashing()
{
	m_DashFov = 2;
	//GetCharacterMovement()->StopMovementImmediately(); //이동 중지 호출
	GetCharacterMovement()->BrakingFrictionFactor = 2.f; //마찰력 초기화
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayerCharacter::ResetDash, m_DashCooldown, false); //dashstop시간 이후 함수 호출
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
	Skill1(); // playercharacter을 상속받은 클래스의 skill1 함수가 호출 (greystone class의 함수가 호출)
}
void APlayerCharacter::Skill2Key()
{
	m_PushedSkillIdx = 2;
	Skill2(); // playercharacter을 상속받은 클래스의 skill2 함수가 호출 (greystone class의 함수가 호출)
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

	// 플레이어가 죽었을 경우
	if (m_PlayerInfo.HP <= 0)
	{
		//m_AnimInstance->ChangeAnimType(EMonsterAnimType::Death);

		//AMonsterAIController* MonsterController = Cast<AMonsterAIController>(GetController());

		//if (MonsterController)
		//	MonsterController->BrainComponent->StopLogic(TEXT("Dead"));
	}
	
	// UI
	AchuchuGameModeBase* GameMode = Cast<AchuchuGameModeBase>(GetWorld()->GetAuthGameMode());

	//캐릭터 hp
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

	//카메라 쉐이트
	//GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(UHitCameraShake::StaticClass());
	ShakeCamera();

	return Damage;
}

void APlayerCharacter::NameWidgetCallback()
{
	//인스턴스에 이름 저장해놓은것 불러와서 ui에 set
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
				//내
				UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());
				const FString playername = GameInst->GetPlayerInfoName();
				CharacterHUD->SetName(playername);
			}
		}
	}
}

void APlayerCharacter::ShakeCamera()
{
	//카메라 쉐이트
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
	// 소켓의 위치를 얻어온다.
	FVector	SocketLoc;

	//스켈레톤에서 소켓이름으로 소켓 가져옴
	if (Left)
		SocketLoc = GetMesh()->GetSocketLocation(TEXT("Foot_L"));

	else
		SocketLoc = GetMesh()->GetSocketLocation(TEXT("Foot_R"));

	// 소켓의 위치에서 아래로 라인충돌을 한다.
	FCollisionQueryParams	params(NAME_None, false, this);
	params.bReturnPhysicalMaterial = true; //피지컬메테리얼을 사용하겟다

	FHitResult	result; //충돌처리
	bool Hit = GetWorld()->LineTraceSingleByChannel(result, SocketLoc, SocketLoc + FVector(0.f, 0.f, -100.f), //-1m 아래
		ECollisionChannel::ECC_GameTraceChannel5, params); //랜드스케이프 충돌체
	//bool Hit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), ); //디버깅용

	if (Hit)
	{
		UPhysicalMaterialBase* Phys = Cast<UPhysicalMaterialBase>(result.PhysMaterial); //부딪힌 녀석의 피직스메테리얼을 갖고옴

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
	m_Attacking = false; //공격중인가?
}
void APlayerCharacter::UseSkill() {}
void APlayerCharacter::UseSkill(int32 Index) {}
void APlayerCharacter::UseSkillFire(int32 Index) {}
void APlayerCharacter::RemoveItem(EEquipType EquipmentType) {}
void APlayerCharacter::EquipItem(EEquipType EquipmentType, const FString& EquipmentPath) {}
#include "../Effect/NormalEffect.h"
void APlayerCharacter::UseItem() 
{

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // 부딪히던 안부딪히던 무조건 불러오도록
	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),GetActorLocation(), GetActorRotation(), param);
	Effect->LoadParticle(TEXT("ParticleSystem'/Game/FantasyVFXCollection/3_ParticleSystem/Buff/BasicBuff/P_BasicBuff_02.P_BasicBuff_02'"));

	PrintViewport(3.f, FColor::Blue, (TEXT("Max hp")));
	m_PlayerInfo.HP = 500;

	// UI
	AchuchuGameModeBase* GameMode = Cast<AchuchuGameModeBase>(GetWorld()->GetAuthGameMode());

	//캐릭터 hp
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
}
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
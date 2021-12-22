// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "MonsterSpawnPoint.h"
#include "../chuchuGameInstance.h"
#include "../chuchuGameModeBase.h"
#include "MonsterAIController.h"
#include "../Effect/HitCameraShake.h"
#include "../UIItem/ItemBox.h"
#include "../UI/HPBar.h"

// Sets default values
AMonster::AMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	m_SpawnPoint = nullptr; //���� ��� ��

	//�ݸ��� 
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision); //�޽��� �浹ó���� ���� �ʰڴ�

	//�̴Ͼ� �����Ͽ��� ai�� ���̴� AutoPossessAI������ ���� ����� ���ų� �����ǰų� �Ѵ� ó�����Ƿ� �Ķ���͸� ��
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//������ �ӵ��� �����ڿ��� �����ع���
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	m_AttackEnd = false;
	m_PatrolIndex = 1;

	SetCanBeDamaged(true); //�������� ���������� ���Ϳ��� �����Ű���ִ�

	//ui 
	m_HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	m_HPBar->SetupAttachment(GetMesh());

	static ConstructorHelpers::FClassFinder<UUserWidget>	HPBarAsset(TEXT("WidgetBlueprint'/Game/UI/UI_HPBar.UI_HPBar_C'"));

	if (HPBarAsset.Succeeded())
		m_HPBar->SetWidgetClass(HPBarAsset.Class);

	m_HPBar->SetWidgetSpace(EWidgetSpace::Screen);
	m_HPBar->SetDrawSize(FVector2D(200.f, 60.f));
	m_HPBar->SetRelativeLocation(FVector(0.f, 0.f, 230.f));
	m_HPBar->SetBlendMode(EWidgetBlendMode::Transparent);

	//���͵鿡�� �׷��̽��� ��Į ��ų ���� �ȹް� false�� äũ��
	GetMesh()->bReceivesDecals = false;

	//M_Dawn_Minion_Master���� ���׸��� ��Ʈ����Ʈ ��Ƽ����  blend��� masked�� ����
	m_Dissolve = 1.5f;
	m_DissolveTime = 5.f;
	m_DissolveAccTime = 0.f;
	m_DissolveEnable = false;
	m_DissolveMin = -1.f;
	m_DissolveMax = 1.5f;

	m_DissolveRange = m_DissolveMax - m_DissolveMin;


	//test
	m_DropItemArray.Add(TEXT("GreatSword"));
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	//������ ������ ���带 ���ؼ� ������ ���̹Ƿ� �����ڿ��� ���� �� ����...... 
	UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());
	
	//������ ������ ���̺�κ��� ������ ����
	if (GameInst)
	{
		const FMonsterTableInfo* Info = GameInst->FindMonsterInfo(m_MonsterInfoName);

		if (Info)
		{
			m_MonsterInfo.Name = Info->Name;
			m_MonsterInfo.Attack = Info->Attack;
			m_MonsterInfo.Armor = Info->Armor;
			m_MonsterInfo.HP = Info->HP;
			m_MonsterInfo.HPMax = Info->HPMax;
			m_MonsterInfo.MP = Info->MP;
			m_MonsterInfo.MPMax = Info->MPMax;
			m_MonsterInfo.Level = Info->Level;
			m_MonsterInfo.Exp = Info->Exp;
			m_MonsterInfo.Gold = Info->Gold;
			m_MonsterInfo.AttackDistance = Info->AttackDistance;
			m_MonsterInfo.AttackSpeed = Info->AttackSpeed;
			m_MonsterInfo.AttackAngle = Info->AttackAngle;
			m_MonsterInfo.MoveSpeed = Info->MoveSpeed;
			m_MonsterInfo.TraceDistance = Info->TraceDistance;
		}

	}

	m_AnimInstance = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance()); //������ �ִϸ��̼� �ν��Ͻ��� �޾Ƴ��� �̰��� �̿��ؼ� �ִ� ü����
	m_HPBarWidget = Cast<UHPBar>(m_HPBar->GetWidget());
	m_HPBarWidget->SetDelegate<AMonster>(this, &AMonster::NameWidgetCallback);

	//m_HPBarWidget->SetName(m_MonsterInfo.Name);
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_DissolveEnable)
	{
		m_DissolveAccTime += DeltaTime;
		
		//�ƽ������� ������ ���� �ð��� ���� ������ ���� ���ϵ���
		m_Dissolve = m_DissolveMax - (m_DissolveAccTime / m_DissolveTime * m_DissolveRange);

		GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Dissolve"), m_Dissolve);

		if (m_DissolveAccTime >= m_DissolveTime)
		{
			m_DissolveAccTime = m_DissolveTime;

			m_Dissolve = m_DissolveMin;

			m_SpawnPoint->Death();
			Destroy();
		}
	}
}
EMonsterAnimType AMonster::GetAnimType()	const
{
	return m_AnimInstance->GetAnimType();
}

void AMonster::ChangeAnimType(EMonsterAnimType Type)
{
	m_AnimInstance->ChangeAnimType(Type);
}

void AMonster::NormalAttack()
{
}


float AMonster::TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent,class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Damage == 0.f)
		return 0.f;

	Damage = Damage - m_MonsterInfo.Armor; //������ ����
	Damage = Damage < 1.f ? 1.f : Damage; //�������� -ó�������ʵ���

	m_MonsterInfo.HP -= Damage; 

	// dead
	if (m_MonsterInfo.HP <= 0)
	{
		m_AnimInstance->ChangeAnimType(EMonsterAnimType::Death);

		AMonsterAIController* MonsterController = Cast<AMonsterAIController>(GetController());

		if (MonsterController)
			MonsterController->BrainComponent->StopLogic(TEXT("Dead")); //ai�� ���� ����

		//���͸� �׿����� ����Ʈ ���� üũ
		AchuchuGameModeBase* gameMode = Cast<AchuchuGameModeBase>(GetWorld()->GetAuthGameMode());
		if (gameMode)
		{
			UQuestWidget* widget = gameMode->GetMainHUD()->GetQuestWidget();
			if (widget)
			{
				widget->QuestCheck(EQuestType::Hunt, m_MonsterInfo.Name);
			}
		}

		//������ ���
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		int32 itemindex = 0;
		UchuchuGameInstance* gameinst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance()); // ���� �ν��Ͻ��� ������ ��̴� �ϳ��� �������
		if (gameinst)
		{

			const FUIItemDataInfo* iteminfo = gameinst->FindUIItemInfo(m_DropItemArray[itemindex]);
			if (iteminfo)
			{
				FActorSpawnParameters itemparam;
				itemparam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				FBox dropbox(GetMesh()->GetComponentLocation() - FVector(50.f, 50.f, -50.f), GetMesh()->GetComponentLocation() + FVector(50.f, 50.f, 100.f)); //Min, Max

				AItemBox* Itembox = GetWorld()->SpawnActor<AItemBox>(AItemBox::StaticClass(), FMath::RandPointInBox(dropbox), GetMesh()->GetComponentRotation(), itemparam);
				if (Itembox)
				{
					Itembox->SetMesh(iteminfo->m_PickMesh);
					Itembox->SetItemName(iteminfo->m_ItemName);
					PrintViewport(10.f, FColor::Blue, FString::Printf(TEXT("Dropitem : %s"), *iteminfo->m_ItemName));
				}

			}
		}
	}

	else //������ �ƴҶ�
		m_AnimInstance->SetHit();

	if (IsValid(m_HPBarWidget))
	{
		m_HPBarWidget->SetHPPercent(m_MonsterInfo.HP / (float)m_MonsterInfo.HPMax);
	}

	return Damage;
}

void AMonster::Death()
{
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DissolveEnable"), 1.f);
	m_DissolveEnable = true;
}

void AMonster::SpawnEnd()
{

}

void AMonster::NameWidgetCallback()
{
	m_HPBarWidget->SetName(m_MonsterInfo.Name);
}

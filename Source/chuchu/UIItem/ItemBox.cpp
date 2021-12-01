// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemBox.h"
#include "../chuchuGameInstance.h"
#include "../chuchuGameModeBase.h"
#include "../Player/MainPlayerController.h"
#include "ItemData.h"

// Sets default values
AItemBox::AItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_ItemBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemBox"));
	SetRootComponent(m_ItemBox);

	m_ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	m_ItemMesh->SetupAttachment(m_ItemBox);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ItemMeshAsset(TEXT("StaticMesh'/Game/Item/Cube2.Cube2'"));
	if (ItemMeshAsset.Succeeded())
		m_ItemMesh->SetStaticMesh(ItemMeshAsset.Object);

	m_ItemMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));

	m_ItemBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	m_ItemBox->SetSimulatePhysics(true);
	m_ItemBox->SetCollisionProfileName(("PlayerTrigger"));

	// �ܰ��� ������
	m_ItemMesh->bRenderCustomDepth = true; //���� ���ٽ� ��� üũ
	m_ItemMesh->CustomDepthStencilWriteMask = ERendererStencilMask::ERSM_Default; //���ٽ� ���� : ����Ʈ
	m_ItemMesh->SetCustomDepthStencilValue(255);
	
	m_ItemOutline = false;
}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();

	//m_ItemBox->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::ItemBeginOverlap); // ������ ����, ������ ��
	// ��������Ʈ�� ������ �Լ��� �����ϰ� ���ε����ش�.
	// �Լ����� �����Ӱ� �ص� ������, �Ű������� ��Ȯ�� ����� �������� �ȴ�.
	// PrimitiveComponent.h 145�ٿ� ����Ǿ�����
	// ���̳��� ��������Ʈ ���ε� AddDynamic�� ���ڸ������� �˻����� �����Ƿ� �׳� �������ϰ� ���� ��.
}

// Called every frame
void AItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_ItemOutline)
	{
		AMainPlayerController* playercontroller = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());
		if (playercontroller)
		{
			if (!playercontroller->IsMouseOn())
			{
				m_ItemMesh->SetCustomDepthStencilValue(255);
				m_ItemOutline = false;
			}
		}
	}

}

void AItemBox::ItemClicked()
{
	UchuchuGameInstance* gameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());
	if (gameInst)
	{
		const FUIItemDataInfo* ItemInfo = gameInst->FindUIItemInfo(m_ItemName);
		if (ItemInfo)
		{
			UItemData* ItemData = NewObject<UItemData>(this, UItemData::StaticClass());
			ItemData->SetNameText(ItemInfo->m_ItemName);
			ItemData->SetIconTex(ItemInfo->m_Thumbnail);
			ItemData->SetMeshPath(ItemInfo->m_MeshPath);
			ItemData->SetItemType(EItemType::Equip, EEquipType::WEAPON);
			ItemData->SetPickMesh(ItemInfo->m_PickMesh);

			AchuchuGameModeBase* gameMode = Cast<AchuchuGameModeBase>(GetWorld()->GetAuthGameMode());
			if (gameMode)
			{
				gameMode->GetMainHUD()->GetInventory()->AddItem(ItemData);
			}
		}

	}
	Destroy();
}

void AItemBox:: ItemOutLineOn()
{
	m_ItemMesh->SetCustomDepthStencilValue(1);
	m_ItemOutline = true;
}

void AItemBox::ItemOutLineOff()
{
	m_ItemOutline = false;
}


/* 	// �÷��̾�� �浹�Ǿ��� �� �̺�Ʈ�� �߻��Ǵ� �Լ�,  ������ ����, ������ ��
*  1. �Ű����� �̸��� ��Ȯ�� ���� �ʾƼ�
*  2. ���� �ν��Ͻ��� ���� ��� ���̽��� �������̷� �ҷ��ͼ��� �ȵ�.. ���� ���� �ҷ���.
void AItemBox::ItemBeginOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UchuchuGameInstance* gameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());
	if (gameInst)
	{
		const FUIItemDataInfo* ItemInfo = gameInst->FindUIItemInfo(m_ItemName);
		if (ItemInfo)
		{
			UItemData* ItemData = NewObject<UItemData>(this, UItemData::StaticClass());
			ItemData->SetNameText(ItemInfo->m_ItemName);
			ItemData->SetIconTex(ItemInfo->m_Thumbnail);
			//ItemData->SetItemtype(ItemInfo->ItemType, ItemInfo->m_EquipType);
			ItemData->SetItemType(EItemType::Equip, EEquipType::WEAPON);
			ItemData->SetIndex(0); //?
			ItemData->SetPickMesh(ItemInfo->m_PickMesh);   //ItemData->SetMeshPath(TEXT("StaticMesh'/Game/GreatSword/GreatSword/Weapon/GreatSword_02.GreatSword_02'")
			//m_InventoryTile->AddItem(ItemData);
			//Items.Add(Data2);

			AchuchuGameModeBase* gameMode = Cast<AchuchuGameModeBase>(GetWorld()->GetAuthGameMode());
			if (gameMode)
			{
				gameMode->GetMainHUD()->GetInventory()->AddItem(ItemData);
			}
		}

	}
	Destroy();
}
*/

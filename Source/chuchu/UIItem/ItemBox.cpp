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

	// 외곽선 렌더링
	m_ItemMesh->bRenderCustomDepth = true; //뎁스 스텐실 사용 체크
	m_ItemMesh->CustomDepthStencilWriteMask = ERendererStencilMask::ERSM_Default; //스텐실 버퍼 : 디폴트
	//m_ItemMesh->SetCustomDepthStencilValue(255); //외각선 off
	m_ItemMesh->SetCustomDepthStencilValue(1);
	
	m_ItemOutline = false;

}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();

	//m_ItemBox->OnComponentEndOverlap.AddDynamic(this, &AItemBox::ItemEndOverlap);
	
	m_ItemBox->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::ItemBeginOverlap); // --> 안됨
	
	// 델리게이트로 실행할 함수를 선언하고 바인딩해준다.
	// 함수명은 자유롭게 해도 되지만, 매개변수를 정확히 써줘야 컴파일이 된다.
	// PrimitiveComponent.h 145줄에 선언되어있음
	// 다이내믹 델리게이트 바인딩 AddDynamic은 인텔리센스에 검색되지 않으므로 그냥 컴파일하고 쓰면 됨.
}

// Called every frame
void AItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	* //if item clicked
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
	*/
}
#include "../Player/PlayerCharacter.h"
// 플레이어와 충돌되었을 때 이벤트가 발생되는 함수 --> 안됨
// 상자의 크기가 커서 완전히 생성되기 전 다른 어떤것과 충돌처리 되어 Destroy함수가 불려 몬스터에서 스폰했을 ㄸㅐ nullptr이 뜨는 것
// 
void AItemBox::ItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (!OverlapCom->GetOwner()->IsA(APlayerCharacter::StaticClass()))
	APlayerCharacter* InPlayer = Cast<APlayerCharacter>(OtherActor);
	if (InPlayer)
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
				ItemData->SetItemType(EItemType::Equip, EEquipType::WEAPON);
				ItemData->SetIndex(0); //?
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

}


void AItemBox::ItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
			ItemData->SetItemType(EItemType::Equip, EEquipType::WEAPON);
			ItemData->SetIndex(0); //?
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

//if click item, pick item
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

void AItemBox::ItemOutLineOn()
{
	m_ItemMesh->SetCustomDepthStencilValue(1);
	m_ItemOutline = true;
}

void AItemBox::ItemOutLineOff()
{
	m_ItemOutline = false;
}
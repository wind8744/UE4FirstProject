// Copyright Epic Games, Inc. All Rights Reserved.


#include "chuchuGameModeBase.h"
#include "chuchuGameInstance.h"
#include "Player/MainPlayerController.h"
AchuchuGameModeBase::AchuchuGameModeBase()
{
	// UClass : UObject 를 상속받은 언리얼엔진 클래스들은 모두 UClass 를 가지고 있다.
	// UClass는 자신이 어떤 클래스 타입인지에 대한 클래스 정보를 가지고 있다.
	// 그래서 언리얼 엔진의 객체를 생성할때는 어떤 타입인지 UClass 정보를 넘겨서 생성하도록 되어 있다.
	// GameMode에서 PlayerController 와 DefaultPawn의 경우 모두 UClass를 지정하게끔 만들어져 있다.
	// ClassFinder를 할 때은 경로의 마지막에  _C를 붙여아 한다.
	// 

	//GhostLady
	
	static ConstructorHelpers::FClassFinder<APawn> KnightClass(TEXT("Blueprint'/Game/Player/GhostLady/BPGhostLady.BPGhostLady_C'"));

	if (KnightClass.Succeeded())
		m_PlayerClassArray.Add(KnightClass.Class);
		
	//AUrora
	/*
	static ConstructorHelpers::FClassFinder<APawn> KnightClass(TEXT("Blueprint'/Game/Player/Aurora/BPAurora.BPAurora_C'")); 
	
	if (KnightClass.Succeeded())
		m_PlayerClassArray.Add(KnightClass.Class);
	*/
	
	//aurora
	static ConstructorHelpers::FClassFinder<APawn> ArcherClass(TEXT("Blueprint'/Game/Player/BPGreystone.BPGreystone_C'"));

	if (ArcherClass.Succeeded())
		m_PlayerClassArray.Add(ArcherClass.Class);

	//magic
	static ConstructorHelpers::FClassFinder<APawn>	MagicionClass(TEXT("Blueprint'/Game/Player/BPGreystone.BPGreystone_C'"));

	if (MagicionClass.Succeeded())
		m_PlayerClassArray.Add(MagicionClass.Class);

	//ui 블프불러오기
	static ConstructorHelpers::FClassFinder<UMainHUD>	MainHUDClass(TEXT("WidgetBlueprint'/Game/UI/UI_MainHUD.UI_MainHUD_C'"));
	if (MainHUDClass.Succeeded())
		m_MainHUDClass = MainHUDClass.Class;

	//플레이어 메인 컨트롤러
	PlayerControllerClass = AMainPlayerController::StaticClass();
}

void AchuchuGameModeBase::InitGame(const FString& MapName,const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// UClass : UObject를 상속받은 언리얼엔진 클래스들은 모두 UClass를 가지고 있다.
	// UClass는 자신이 어떤 클래스 타입인지에 대한 클래스 정보를 가지고 있다.
	// 그래서 언리얼엔진의 객체를 생성할때는 어떤 타입인지 UClass 정보를 넘겨서 생성하도
	// 록 되어 있다.
	// GameMode에서 PlayerController와 DefaultPawn의 경우 모두 UClass를 지정하게끔
	// 만들어져 있다.
	// ClassFinder를 할때는 경로의 마지막에 _C를 반드시 붙여야 한다.
	UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());

	//게임 씬에서 선택한 직업에 따라 폰이 달라짐
	if (GameInst)
	{
		EPlayerJob SelectJob = GameInst->GetSelectJob();

		switch (SelectJob)
		{
		case EPlayerJob::Knight:
		{
			DefaultPawnClass = m_PlayerClassArray[0];
		}
		break;
		case EPlayerJob::Archer:
		{
			DefaultPawnClass = m_PlayerClassArray[1];
		}
		break;
		case EPlayerJob::Magicion:
		{
			DefaultPawnClass = m_PlayerClassArray[2];
		}
		break;
		}
	}

	//에셋패스를 바로 얻어오루 수 있음
	//에셋 매니저에서 만든 이름과 같아야 함. 
	//에셋들을 경로에서 갖고온것을 구조체로 만들어서 어래이에 저장해둠 그걸 갖고오는 것
	m_MainAssetPath = UAssetPathMain::StaticClass()->GetDefaultObject<UAssetPathMain>();
	m_MainAssetPath->ConvertPath();


}
#include "Player/GhostLady.h"
void AchuchuGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if(LevelName.Equals(TEXT("Main2"), ESearchCase::CaseSensitive)) // FString 비교할 떄 사용
	{
		AGhostLady* Player = Cast<AGhostLady>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Player)
		{
			Player->EquipItem(EEquipType::TOPBODY, TEXT("SkeletalMesh'/Game/GhostLady_S2/Meshes/Characters/Separates/TopBodies/SK_TopBodyD_C.SK_TopBodyD_C'"));
			Player->EquipItem(EEquipType::BOTBODY, TEXT("SkeletalMesh'/Game/GhostLady_S2/Meshes/Characters/Separates/BotBodies/SK_BotBodyD_B.SK_BotBodyD_B'"));
			Player->EquipItem(EEquipType::BOOTS, TEXT("SkeletalMesh'/Game/GhostLady_S2/Meshes/Characters/Separates/Shoes/SK_Boots_D.SK_Boots_D'"));
			Player->EquipItem(EEquipType::HAND, TEXT("SkeletalMesh'/Game/GhostLady_S2/Meshes/Characters/Separates/Gauntlets/SK_GauntletsMerge_D.SK_GauntletsMerge_D'"));
			Player->EquipItem(EEquipType::WEAPON, TEXT("StaticMesh'/Game/GreatSword/GreatSword/Weapon/GreatSword_02.GreatSword_02'"));
		}
	}

	if (IsValid(m_MainHUDClass))
	{
		m_MainHUD = Cast<UMainHUD>(CreateWidget(GetWorld(),m_MainHUDClass));

		if (m_MainHUD)
			m_MainHUD->AddToViewport(); //뷰포트에 붙여라
	}

	/* Ori
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();

	FInputModeGameAndUI	Mode;
	Controller->SetInputMode(Mode);
	Controller->bShowMouseCursor = true;
	*/

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	Controller->SetInputMode(FInputModeGameOnly()); // 커서 없어지고 마우스 방향으로 카메라 회전

}

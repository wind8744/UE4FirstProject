// Copyright Epic Games, Inc. All Rights Reserved.


#include "chuchuGameModeBase.h"
#include "chuchuGameInstance.h"
#include "Player/GhostLady.h"
#include "Player/MainPlayerController.h"

AchuchuGameModeBase::AchuchuGameModeBase()
{
	// UClass : UObject �� ��ӹ��� �𸮾��� Ŭ�������� ��� UClass �� ������ �ִ�.
	// UClass�� �ڽ��� � Ŭ���� Ÿ�������� ���� Ŭ���� ������ ������ �ִ�.
	// �׷��� �𸮾� ������ ��ü�� �����Ҷ��� � Ÿ������ UClass ������ �Ѱܼ� �����ϵ��� �Ǿ� �ִ�.
	// GameMode���� PlayerController �� DefaultPawn�� ��� ��� UClass�� �����ϰԲ� ������� �ִ�.
	// ClassFinder�� �� ���� ����� ��������  _C�� �ٿ��� �Ѵ�.

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

	//ui �����ҷ�����
	static ConstructorHelpers::FClassFinder<UMainHUD>	MainHUDClass(TEXT("WidgetBlueprint'/Game/UI/UI_MainHUD.UI_MainHUD_C'"));
	if (MainHUDClass.Succeeded())
		m_MainHUDClass = MainHUDClass.Class;

	//�÷��̾� ���� ��Ʈ�ѷ�
	PlayerControllerClass = AMainPlayerController::StaticClass();
}

void AchuchuGameModeBase::InitGame(const FString& MapName,const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// UClass : UObject�� ��ӹ��� �𸮾��� Ŭ�������� ��� UClass�� ������ �ִ�.
	// UClass�� �ڽ��� � Ŭ���� Ÿ�������� ���� Ŭ���� ������ ������ �ִ�.
	// �׷��� �𸮾����� ��ü�� �����Ҷ��� � Ÿ������ UClass ������ �Ѱܼ� �����ϵ�
	// �� �Ǿ� �ִ�.
	// GameMode���� PlayerController�� DefaultPawn�� ��� ��� UClass�� �����ϰԲ�
	// ������� �ִ�.
	// ClassFinder�� �Ҷ��� ����� �������� _C�� �ݵ�� �ٿ��� �Ѵ�.	
	UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());
	
	//���� ������ ������ ������ ���� ���� �޶���
	if (GameInst)
	{
		DefaultPawnClass = m_PlayerClassArray[0];
	}

	//�����н��� �ٷ� ������ �� ����
	//���� �Ŵ������� ���� �̸��� ���ƾ� ��. 
	//���µ��� ��ο��� ����°��� ����ü�� ���� ��̿� �����ص� �װ� ������� ��
	m_MainAssetPath = UAssetPathMain::StaticClass()->GetDefaultObject<UAssetPathMain>();
	m_MainAssetPath->ConvertPath();
}

void AchuchuGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if(LevelName.Equals(TEXT("Main2"), ESearchCase::CaseSensitive)) // FString ���� �� ���
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
			m_MainHUD->AddToViewport(); //����Ʈ�� �ٿ���
	}

	/* Ori
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	FInputModeGameAndUI	Mode;
	Controller->SetInputMode(Mode);
	Controller->bShowMouseCursor = true;
	*/

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	Controller->SetInputMode(FInputModeGameOnly()); // Ŀ�� �������� ���콺 �������� ī�޶� ȸ��

}

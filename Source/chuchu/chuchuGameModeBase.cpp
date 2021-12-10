// Copyright Epic Games, Inc. All Rights Reserved.


#include "chuchuGameModeBase.h"
#include "chuchuGameInstance.h"
#include "Player/MainPlayerController.h"
AchuchuGameModeBase::AchuchuGameModeBase()
{
	// UClass : UObject �� ��ӹ��� �𸮾��� Ŭ�������� ��� UClass �� ������ �ִ�.
	// UClass�� �ڽ��� � Ŭ���� Ÿ�������� ���� Ŭ���� ������ ������ �ִ�.
	// �׷��� �𸮾� ������ ��ü�� �����Ҷ��� � Ÿ������ UClass ������ �Ѱܼ� �����ϵ��� �Ǿ� �ִ�.
	// GameMode���� PlayerController �� DefaultPawn�� ��� ��� UClass�� �����ϰԲ� ������� �ִ�.
	// ClassFinder�� �� ���� ����� ��������  _C�� �ٿ��� �Ѵ�.
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

	//ui �����ҷ�����
	static ConstructorHelpers::FClassFinder<UMainHUD>	MainHUDClass(TEXT("WidgetBlueprint'/Game/UI/UI_MainHUD.UI_MainHUD_C'"));
	if (MainHUDClass.Succeeded())
		m_MainHUDClass = MainHUDClass.Class;

	//�÷��̾� ���� ��Ʈ�ѷ�
	PlayerControllerClass = AMainPlayerController::StaticClass();
}

void AchuchuGameModeBase::InitGame(const FString& MapName,
	const FString& Options, FString& ErrorMessage)
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

	//�����н��� �ٷ� ������ �� ����
	//���� �Ŵ������� ���� �̸��� ���ƾ� ��. 
	//���µ��� ��ο��� ����°��� ����ü�� ���� ��̿� �����ص� �װ� ������� ��
	m_MainAssetPath = UAssetPathMain::StaticClass()->GetDefaultObject<UAssetPathMain>();
	m_MainAssetPath->ConvertPath();


}

void AchuchuGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(m_MainHUDClass))
	{
		m_MainHUD = Cast<UMainHUD>(CreateWidget(GetWorld(),
			m_MainHUDClass));

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

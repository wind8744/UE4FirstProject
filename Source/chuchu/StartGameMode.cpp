// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameMode.h"


AStartGameMode::AStartGameMode()
{
	//start BP ��������
	static ConstructorHelpers::FClassFinder<UStartWidget>	StartHUDClass(TEXT("WidgetBlueprint'/Game/UI/UI_Start.UI_Start_C'"));

	if (StartHUDClass.Succeeded())

		m_StartHUDClass = StartHUDClass.Class;
}

void AStartGameMode::InitGame(const FString& MapName,
	const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AStartGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(m_StartHUDClass))
	{

		m_StartHUD = Cast<UStartWidget>(CreateWidget(GetWorld(),
			m_StartHUDClass));

		if (m_StartHUD)
			m_StartHUD->AddToViewport();
	}

	//���� �ڵ�
	//���콺 Ŀ���� ���̰� �Ⱥ��̰� �۾��� �� �ִ�.
	APlayerController* Controller = GetWorld()->GetFirstPlayerController(); //

	FInputModeUIOnly	Mode; //
	//FInputModeGameOnly //����Ʈ , ���� �Ǿ����� -> ���콺�� �������� ����Ʈ ������ �ȳ���
	//FInputModeGameAndUI //���콺�� �����ִ°� ,Ŀ�� Ŭ�� ����, Ŀ�� ������ �̵�

	Controller->SetInputMode(Mode);
	Controller->bShowMouseCursor = true; //���콺 Ŀ���� ��������, ���̵���
	
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameMode.h"


AStartGameMode::AStartGameMode()
{
	//start BP 가져오기
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

	//원래 코드
	//마우스 커서가 보이고 안보이고를 작업할 수 있다.
	APlayerController* Controller = GetWorld()->GetFirstPlayerController(); //

	FInputModeUIOnly	Mode; //
	//FInputModeGameOnly //디폴트 , 현재 되어있음 -> 마우스가 없어지고 뷰포트 밖으로 안나옴
	//FInputModeGameAndUI //마우스가 나와있는것 ,커서 클릭 가능, 커서 밖으로 이동

	Controller->SetInputMode(Mode);
	Controller->bShowMouseCursor = true; //마우스 커서가 나오도록, 보이도록
	
}


#include "SelectPlayerController.h"
#include "SelectPlayer.h"
#include "../CharacterSelectGameMode.h"
#include "../UI/CharacterSelectHUD.h"
#include "../chuchuGameInstance.h"

ASelectPlayerController::ASelectPlayerController()
{
	bShowMouseCursor = true;
	m_LButtonClick = false;
	m_SelectJob = EPlayerJob::End;

	m_SelectCharacter = nullptr;
}


void ASelectPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	Picking(); //틱에서 실시간으로 처리
}

void ASelectPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//눌렀을때
	InputComponent->BindAction(TEXT("CharacterSelect"), //프로젝트 세팅 입력에서 셋팅해줌
		EInputEvent::IE_Pressed, this, &ASelectPlayerController::MouseClick);

	//뗄때
	InputComponent->BindAction(TEXT("CharacterSelect"),
		EInputEvent::IE_Released, this, &ASelectPlayerController::MouseRelease);
}

void ASelectPlayerController::MouseClick()
{
	m_LButtonClick = true;

	//Picking(); 틱에서 실시간으로 처리

	if (m_SelectCharacter) //선택된게 있을대 처리
	{
		ACharacterSelectGameMode* GameMode =
			Cast<ACharacterSelectGameMode>(GetWorld()->GetAuthGameMode());

		if (GameMode)
		{
			ASelectPlayer* SelectPlayer = Cast<ASelectPlayer>(m_SelectCharacter);

			UCharacterSelectHUD* SelectHUD = GameMode->GetSelectHUD();

			if (SelectHUD)
			{
				SelectHUD->EnableStartButton(true);

				m_SelectJob = SelectPlayer->GetPlayerJob();

				UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());

				if (GameInst)
				{
					GameInst->SetSelectJob(m_SelectJob);
				}

				SelectHUD->SetInputNameVisibility(ESlateVisibility::SelfHitTestInvisible);

				switch (SelectPlayer->GetPlayerJob())
				{
				case EPlayerJob::Knight:
					SelectHUD->SetKnightStateVisibility(ESlateVisibility::SelfHitTestInvisible);
					SelectHUD->SetArcherStateVisibility(ESlateVisibility::Collapsed);
					break;
				case EPlayerJob::Archer:
					SelectHUD->SetKnightStateVisibility(ESlateVisibility::Collapsed);
					SelectHUD->SetArcherStateVisibility(ESlateVisibility::SelfHitTestInvisible);
					break;
				case EPlayerJob::Magicion:
					break;
				}
			}
		}
	}

	else //순수 마우스 피킹
	{
		ACharacterSelectGameMode* GameMode =
			Cast<ACharacterSelectGameMode>(GetWorld()->GetAuthGameMode());

		if (GameMode)
		{
			UCharacterSelectHUD* SelectHUD = GameMode->GetSelectHUD();

			if (SelectHUD)
			{
				SelectHUD->SetInputNameVisibility(ESlateVisibility::Collapsed);
				SelectHUD->EnableStartButton(false);
				SelectHUD->SetKnightStateVisibility(ESlateVisibility::Collapsed);
				SelectHUD->SetArcherStateVisibility(ESlateVisibility::Collapsed);
			}
		}
	}

}

void ASelectPlayerController::MouseRelease()
{
	m_LButtonClick = false;
}

void ASelectPlayerController::Picking()
{
	FHitResult	result;
	// 커서 아래 있는것 찾아라 (충돌체처럼)

	bool Hit = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel9, //마우스 충돌체 defaulteditor .ini에서 
		false, result);

	// 디폴트 폰을 바꾸었고 선택된 구체(폰) 안에서 마우스를 누르는 것이므로 충돌 처리가 일어나지 않는다 ( 출력 로그에 뜨지 앟ㄴ느다.)
	// 앞에 원하는 캐릭터들이나 여러가지를 배치해 놓고 사용하면 된다.
	if (Hit)
	{
		//LOG(TEXT("%s"), *result.GetActor()->GetName());
		ASelectPlayer* SelectPlayer = Cast<ASelectPlayer>(result.GetActor());

		if (SelectPlayer)
		{
			if (m_SelectCharacter != SelectPlayer)
			{
				if (m_SelectCharacter) //선택된 캐릭터가 존재할 때 
					m_SelectCharacter->GetMesh()->SetCustomDepthStencilValue(255); //255번

				m_SelectCharacter = SelectPlayer; //교체

				m_SelectCharacter->GetMesh()->SetCustomDepthStencilValue(1); //1번으로 변경

				PrintViewport(1.f, FColor::Red, TEXT("Hit"));
			}
		}

		else
		{
			if (m_SelectCharacter)
			{
				m_SelectCharacter->GetMesh()->SetCustomDepthStencilValue(255);
				m_SelectCharacter = nullptr;
			}
		}

		//// 커서 아래 눌린 캐릭터
		//LOG(TEXT("%s"), *result.GetActor()->GetName()); //클릭하면 출력 로그
		//ASelectPlayer* SelectPlayer = Cast<ASelectPlayer>(result.GetActor());

		//if (SelectPlayer)
		//{
		//	ACharacterSelectGameMode* GameMode =
		//		Cast<ACharacterSelectGameMode>(GetWorld()->GetAuthGameMode());

		//	if (GameMode)
		//	{
		//		UCharacterSelectHUD* SelectHUD = GameMode->GetSelectHUD();

		//		if (SelectHUD)
		//		{
		//			SelectHUD->EnableStartButton(true);

		//			m_SelectJob = SelectPlayer->GetPlayerJob();

		//			UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());

		//			if (GameInst)
		//			{
		//				GameInst->SetSelectJob(m_SelectJob);
		//			}

		//			SelectHUD->SetInputNameVisibility(ESlateVisibility::SelfHitTestInvisible);

		//			//상대방 것은 꺼지고 선택된 것이 나올것

		//			switch (SelectPlayer->GetPlayerJob())
		//			{
		//			case EPlayerJob::Knight:
		//				SelectHUD->SetKnightStateVisibility(ESlateVisibility::SelfHitTestInvisible);
		//				SelectHUD->SetArcherStateVisibility(ESlateVisibility::Collapsed);
		//				break;
		//			case EPlayerJob::Archer:
		//				SelectHUD->SetKnightStateVisibility(ESlateVisibility::Collapsed);
		//				SelectHUD->SetArcherStateVisibility(ESlateVisibility::SelfHitTestInvisible);
		//				break;
		//			case EPlayerJob::Magicion:
		//				break;
		//			}
		//		}
		//	}

	}

	//캐릭터가 눌린것이 아닐때
	else
	{
		if (m_SelectCharacter)
		{
			m_SelectCharacter->GetMesh()->SetCustomDepthStencilValue(255);
			m_SelectCharacter = nullptr;
		}
		//ACharacterSelectGameMode* GameMode =
		//	Cast<ACharacterSelectGameMode>(GetWorld()->GetAuthGameMode());

		//if (GameMode)
		//{
		//	UCharacterSelectHUD* SelectHUD = GameMode->GetSelectHUD();

		//	if (SelectHUD)
		//	{
		//		SelectHUD->SetInputNameVisibility(ESlateVisibility::Collapsed);
		//		SelectHUD->EnableStartButton(false);
		//		SelectHUD->SetKnightStateVisibility(ESlateVisibility::Collapsed);
		//		SelectHUD->SetArcherStateVisibility(ESlateVisibility::Collapsed);
		//	}
		//}
	}

}


//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "SelectPlayerController.h"
//#include "SelectPlayer.h"
//#include "../CharacterSelectGameMode.h"
//#include "../UI/CharacterSelectHUD.h"
//#include "../chuchuGameInstance.h"
//
//ASelectPlayerController::ASelectPlayerController()
//{
//	bShowMouseCursor = true;
//	m_LButtonClick = false;
//	m_SelectJob = EPlayerJob::End;
//
//	m_SelectCharacter = nullptr;
//}
//
//
//void ASelectPlayerController::PlayerTick(float DeltaTime)
//{
//	Super::PlayerTick(DeltaTime);
//
//	Picking();
//}
//
//void ASelectPlayerController::SetupInputComponent()
//{
//	Super::SetupInputComponent();
//
//	InputComponent->BindAction(TEXT("CharacterSelect"),
//		EInputEvent::IE_Pressed, this, &ASelectPlayerController::MouseClick);
//
//	InputComponent->BindAction(TEXT("CharacterSelect"),
//		EInputEvent::IE_Released, this, &ASelectPlayerController::MouseRelease);
//}
//
//void ASelectPlayerController::MouseClick()
//{
//	m_LButtonClick = true;
//
//	if (m_SelectCharacter)
//	{
//		ACharacterSelectGameMode* GameMode =
//			Cast<ACharacterSelectGameMode>(GetWorld()->GetAuthGameMode());
//
//		if (GameMode)
//		{
//			ASelectPlayer* SelectPlayer = Cast<ASelectPlayer>(m_SelectCharacter);
//
//			UCharacterSelectHUD* SelectHUD = GameMode->GetSelectHUD();
//
//			if (SelectHUD)
//			{
//				SelectHUD->EnableStartButton(true);
//
//				m_SelectJob = SelectPlayer->GetPlayerJob();
//
//				UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());
//
//				if (GameInst)
//				{
//					GameInst->SetSelectJob(m_SelectJob);
//				}
//
//				SelectHUD->SetInputNameVisibility(ESlateVisibility::SelfHitTestInvisible);
//
//				switch (SelectPlayer->GetPlayerJob())
//				{
//				case EPlayerJob::Knight:
//					SelectHUD->SetKnightStateVisibility(ESlateVisibility::SelfHitTestInvisible);
//					SelectHUD->SetArcherStateVisibility(ESlateVisibility::Collapsed);
//					break;
//				case EPlayerJob::Archer:
//					SelectHUD->SetKnightStateVisibility(ESlateVisibility::Collapsed);
//					SelectHUD->SetArcherStateVisibility(ESlateVisibility::SelfHitTestInvisible);
//					break;
//				case EPlayerJob::Magicion:
//					break;
//				}
//			}
//		}
//	}
//
//	else
//	{
//		ACharacterSelectGameMode* GameMode =
//			Cast<ACharacterSelectGameMode>(GetWorld()->GetAuthGameMode());
//
//		if (GameMode)
//		{
//			UCharacterSelectHUD* SelectHUD = GameMode->GetSelectHUD();
//
//			if (SelectHUD)
//			{
//				SelectHUD->SetInputNameVisibility(ESlateVisibility::Collapsed);
//				SelectHUD->EnableStartButton(false);
//				SelectHUD->SetKnightStateVisibility(ESlateVisibility::Collapsed);
//				SelectHUD->SetArcherStateVisibility(ESlateVisibility::Collapsed);
//			}
//		}
//	}
//}
//
//void ASelectPlayerController::MouseRelease()
//{
//	m_LButtonClick = false;
//}
//
//void ASelectPlayerController::Picking()
//{
//	FHitResult	result;
//
//	bool Hit = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel9,
//		false, result);
//
//	if (Hit)
//	{
//		//LOG(TEXT("%s"), *result.GetActor()->GetName());
//		ASelectPlayer* SelectPlayer = Cast<ASelectPlayer>(result.GetActor());
//
//		if (SelectPlayer)
//		{
//			if (m_SelectCharacter != SelectPlayer)
//			{
//				if (m_SelectCharacter) //선택된 캐릭터가 존재할 때 
//					m_SelectCharacter->GetMesh()->SetCustomDepthStencilValue(255); //255번
//
//				m_SelectCharacter = SelectPlayer; //교체
//
//				m_SelectCharacter->GetMesh()->SetCustomDepthStencilValue(1); //피킹 시 1번으로 변경
//
//				PrintViewport(1.f, FColor::Red, TEXT("Hit"));
//			}
//		}
//
//		else
//		{
//			if (m_SelectCharacter)
//			{
//				m_SelectCharacter->GetMesh()->SetCustomDepthStencilValue(255);
//				m_SelectCharacter = nullptr;
//			}
//		}
//	}
//
//	else
//	{
//		if (m_SelectCharacter)
//		{
//			m_SelectCharacter->GetMesh()->SetCustomDepthStencilValue(255);
//			m_SelectCharacter = nullptr;
//		}
//	}
//}
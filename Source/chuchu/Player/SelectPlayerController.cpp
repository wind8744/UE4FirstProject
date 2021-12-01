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

	Picking(); //ƽ���� �ǽð����� ó��
}

void ASelectPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//��������
	InputComponent->BindAction(TEXT("CharacterSelect"), //������Ʈ ���� �Է¿��� ��������
		EInputEvent::IE_Pressed, this, &ASelectPlayerController::MouseClick);

	//����
	InputComponent->BindAction(TEXT("CharacterSelect"),
		EInputEvent::IE_Released, this, &ASelectPlayerController::MouseRelease);
}

void ASelectPlayerController::MouseClick()
{
	m_LButtonClick = true;

	//Picking(); ƽ���� �ǽð����� ó��

	if (m_SelectCharacter) //���õȰ� ������ ó��
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

	else //���� ���콺 ��ŷ
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
	// Ŀ�� �Ʒ� �ִ°� ã�ƶ� (�浹üó��)

	bool Hit = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel9, //���콺 �浹ü defaulteditor .ini���� 
		false, result);

	// ����Ʈ ���� �ٲپ��� ���õ� ��ü(��) �ȿ��� ���콺�� ������ ���̹Ƿ� �浹 ó���� �Ͼ�� �ʴ´� ( ��� �α׿� ���� �ۤ�����.)
	// �տ� ���ϴ� ĳ���͵��̳� ���������� ��ġ�� ���� ����ϸ� �ȴ�.
	if (Hit)
	{
		//LOG(TEXT("%s"), *result.GetActor()->GetName());
		ASelectPlayer* SelectPlayer = Cast<ASelectPlayer>(result.GetActor());

		if (SelectPlayer)
		{
			if (m_SelectCharacter != SelectPlayer)
			{
				if (m_SelectCharacter) //���õ� ĳ���Ͱ� ������ �� 
					m_SelectCharacter->GetMesh()->SetCustomDepthStencilValue(255); //255��

				m_SelectCharacter = SelectPlayer; //��ü

				m_SelectCharacter->GetMesh()->SetCustomDepthStencilValue(1); //1������ ����

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

		//// Ŀ�� �Ʒ� ���� ĳ����
		//LOG(TEXT("%s"), *result.GetActor()->GetName()); //Ŭ���ϸ� ��� �α�
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

		//			//���� ���� ������ ���õ� ���� ���ð�

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

	//ĳ���Ͱ� �������� �ƴҶ�
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
//				if (m_SelectCharacter) //���õ� ĳ���Ͱ� ������ �� 
//					m_SelectCharacter->GetMesh()->SetCustomDepthStencilValue(255); //255��
//
//				m_SelectCharacter = SelectPlayer; //��ü
//
//				m_SelectCharacter->GetMesh()->SetCustomDepthStencilValue(1); //��ŷ �� 1������ ����
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
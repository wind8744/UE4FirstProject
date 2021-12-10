// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "PlayerCharacter.h"
#include "../chuchuGameModeBase.h"
#include "../UIItem/ItemBox.h"
#include "../Environment/EnvironmentNormalActor.h"

AMainPlayerController::AMainPlayerController()
{
	//bShowMouseCursor = true; // ui ���� ��Ÿ������ 
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchOverEvents = true;
	//m_MouseClicked = false;
	m_MouseOn = false;
	m_IsUIMode = false;
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("UI"), EInputEvent::IE_Pressed, this, &AMainPlayerController::UIKey);
	//InputComponent->BindAction(TEXT("PickItem"), EInputEvent::IE_Pressed, this, &AMainPlayerController::MouseClick);
	//InputComponent->BindAction(TEXT("PickItem"), EInputEvent::IE_Released, this, &AMainPlayerController::MouseReleased);
}

void AMainPlayerController::UIKey()
{
	m_IsUIMode = m_IsUIMode == true ? false : true;

	if (m_IsUIMode)
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
		AchuchuGameModeBase* chuMode = Cast<AchuchuGameModeBase>(GetWorld()->GetAuthGameMode());
		chuMode->GetMainHUD()->PopupUI();
	}
}


void AMainPlayerController:: MouseClick()
{
	//m_MouseClicked = true;
	PickingItem();
}
void AMainPlayerController::MouseReleased()
{
	//m_MouseClicked = false;
}


void AMainPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// ������ �޽� �ܰ��� ó��
	/* //���콺�� ������ �÷��� �� �ܰ��� ó��, ���콺 �������Ƿ� �ּ�ó���� 
	FHitResult hitResult;
	bool Hit12 = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel9, //���콺 �浹ü defaulteditor .ini���� 
		false, hitResult);
	if (Hit12)
	{
		AItemBox* ItemBox = Cast<AItemBox>(hitResult.GetActor());
		if (ItemBox)
		{
			ItemBox->ItemOutLineOn();
			m_MouseOn = true;
		}
		else 
			m_MouseOn = false;
	}
	*/

	//�ǹ� �浹ü �����ͼ� ĳ���� �Ƿ翧 �����ֱ� 10_12����
	int a = 1;

	for (auto& CollisionActor : m_CollisionEnvironmentActor)
	{
		CollisionActor->SetCameraCollision(false);
	}

	APlayerCharacter* PlayerActor = Cast<APlayerCharacter>(GetPawn());

	FVector	CameraLoc = PlayerActor->GetCameraLocation();
	FVector	PlayerLoc = GetPawn()->GetActorLocation();

	FCollisionQueryParams	params(NAME_None, false, this);

	TArray<FHitResult>	HitResultArray; //�ٸ� �ǹ��� ������ �� ���� �� �����Ƿ�
	bool Hit = GetWorld()->LineTraceMultiByChannel(HitResultArray, CameraLoc, //�̱��� �ƴ϶� ��Ƽ��
		PlayerLoc, ECollisionChannel::ECC_Camera,
		params);


	//ĳ���� �� ī�޶� ���̿� ���� (�ǹ���) �ε����� ��
	if (Hit)
	{
		for (auto& result : HitResultArray)
		{
			AEnvironmentNormalActor* EnvironmentActor =
				Cast<AEnvironmentNormalActor>(result.GetActor());

			if (EnvironmentActor)
			{
				if (EnvironmentActor->GetDepthSilhouette())
				{
					EnvironmentActor->SetCameraCollision(true); //�ݸ����� ������ ���� 1�� 
					PrintViewport(1.f, FColor::Red, TEXT("Hit"));
				}

				bool	Overlap = false;

				for (auto& CollisionActor : m_CollisionEnvironmentActor)
				{
					if (CollisionActor == EnvironmentActor)
					{
						Overlap = true;
						break;
					}
				}

				if (!Overlap)
				{
					m_CollisionEnvironmentActor.Add(EnvironmentActor);
				}
			}
		}
	}

	/*for (auto& CollisionActor : m_CollisionEnvironmentActor)
	{
		if (!CollisionActor->GetCameraCollision())
		{
			m_CollisionEnvironmentActor.Remove(CollisionActor);
			PrintViewport(1.f, FColor::Red, TEXT("Remove"));
		}
	}*/

	for (int32 i = 0; i < m_CollisionEnvironmentActor.Num();)
	{
		if (!m_CollisionEnvironmentActor[i]->GetCameraCollision())
		{
			//m_CollisionEnvironmentActor.Remove(m_CollisionEnvironmentActor[i]); //����� �������� ������ ������ -> error
			m_CollisionEnvironmentActor.RemoveAt(i);//
		}

		else
			++i;
	}
}

void AMainPlayerController::PickingItem()
{
	FHitResult hitResult;
	bool Hit = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel9, //���콺 �浹ü defaulteditor .ini���� 
		false, hitResult);

	if (Hit)
	{
		//UE_LOG(LogTemp, Log, TEXT("NotifyActorOnClicked"));
		//PrintViewport(1.f, FColor::Green, TEXT("NotifyActorOnClicked"));
		//LOG(TEXT("%s"), *result.GetActor()->GetName());
		AItemBox* ItemBox = Cast<AItemBox>(hitResult.GetActor());
		if (ItemBox)
		{
			//PrintViewport(1.f, FColor::Red, TEXT("NotifyActorOnClicked"));
			//UE_LOG(LogTemp, Log, TEXT("ITembooboboboboxxxx"));
			ItemBox->ItemClicked();
		}
	}

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "PlayerCharacter.h"
#include "../UIItem/ItemBox.h"
#include "../Environment/EnvironmentNormalActor.h"

AMainPlayerController::AMainPlayerController()
{
	//bShowMouseCursor = true; // ui 때만 나타나도록 
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchOverEvents = true;
	//m_MouseClicked = false;
	m_MouseOn = false;
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAction(TEXT("PickItem"), EInputEvent::IE_Pressed, this, &AMainPlayerController::MouseClick);
	//InputComponent->BindAction(TEXT("PickItem"), EInputEvent::IE_Released, this, &AMainPlayerController::MouseReleased);
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

	// 아이템 메쉬 외각선 처리
	FHitResult hitResult;
	bool Hit12 = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel9, //마우스 충돌체 defaulteditor .ini에서 
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

	//건물 충돌체 가져와서 캐릭터 실루엣 보여주기 10_12강의
	int a = 1;

	for (auto& CollisionActor : m_CollisionEnvironmentActor)
	{
		CollisionActor->SetCameraCollision(false);
	}

	APlayerCharacter* PlayerActor = Cast<APlayerCharacter>(GetPawn());

	FVector	CameraLoc = PlayerActor->GetCameraLocation();
	FVector	PlayerLoc = GetPawn()->GetActorLocation();

	FCollisionQueryParams	params(NAME_None, false, this);

	TArray<FHitResult>	HitResultArray; //다른 건물도 여러개 가 들어올 수 있으므로
	bool Hit = GetWorld()->LineTraceMultiByChannel(HitResultArray, CameraLoc, //싱글이 아니라 멀티로
		PlayerLoc, ECollisionChannel::ECC_Camera,
		params);


	//캐릭터 와 카메라 사이에 무언가 (건물이) 부딪혔을 때
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
					EnvironmentActor->SetCameraCollision(true); //콜리전이 있으면 값을 1로 
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
			//m_CollisionEnvironmentActor.Remove(m_CollisionEnvironmentActor[i]); //지우면 다음것이 앞으로 땡겨짐 -> error
			m_CollisionEnvironmentActor.RemoveAt(i);//
		}

		else
			++i;
	}
}

void AMainPlayerController::PickingItem()
{
	FHitResult hitResult;
	bool Hit = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel9, //마우스 충돌체 defaulteditor .ini에서 
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectPlayer.h"
#include "../chuchuGameInstance.h"
#include "../CharacterSelectGameMode.h"
#include "../UI/CharacterSelectHUD.h"
#include "../UI/CharacterSelectState.h"

// Sets default values
ASelectPlayer::ASelectPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->bRenderCustomDepth = true;
	GetMesh()->CustomDepthStencilWriteMask = ERendererStencilMask::ERSM_Default;
	GetMesh()->SetCustomDepthStencilValue(255);
}

// Called when the game starts or when spawned
void ASelectPlayer::BeginPlay()
{
	Super::BeginPlay();
	//데이터 에셋은 월드를 통해서 얻어오는 것이므로 생성자에서 쓰일 수 없다...... 
	UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());

	//몬스터의 데이터 테이블로부터 정보를 들고옴
	if (GameInst)
	{
		FString	JobName;

		switch (m_Job)
		{
		case EPlayerJob::Knight:
			JobName = "Knight";
			break;
		case EPlayerJob::Archer:
			JobName = "Archer";
			break;
		case EPlayerJob::Magicion:
			JobName = "Magicion";
			break;
		}

		const FPlayerTableInfo* Info = GameInst->FindPlayerInfo(JobName); //행 이름으로 찾아옴 

		m_PlayerInfo.Job = m_Job;

		if (Info)
		{
			m_PlayerInfo.Name = Info->Name;
			m_PlayerInfo.Attack = Info->Attack;
			m_PlayerInfo.Armor = Info->Armor;
			m_PlayerInfo.HP = Info->HP;
			m_PlayerInfo.HPMax = Info->HPMax;
			m_PlayerInfo.MP = Info->MP;
			m_PlayerInfo.MPMax = Info->MPMax;
			m_PlayerInfo.Level = Info->Level;
			m_PlayerInfo.Exp = Info->Exp;
			m_PlayerInfo.Gold = Info->Gold;
			m_PlayerInfo.AttackDistance = Info->AttackDistance;
			m_PlayerInfo.AttackSpeed = Info->AttackSpeed;
			m_PlayerInfo.AttackAngle = Info->AttackAngle;
			m_PlayerInfo.MoveSpeed = Info->MoveSpeed;

			ACharacterSelectGameMode* GameMode = Cast<ACharacterSelectGameMode>(GetWorld()->GetAuthGameMode());

			UCharacterSelectHUD* SelectHUD = GameMode->GetSelectHUD();

			switch (m_Job)
			{
			case EPlayerJob::Knight:
				SelectHUD->SetKnightInfo(m_PlayerInfo); //UI SelectHUD에서 캐릭 정보 set
				break;
			case EPlayerJob::Archer:
				SelectHUD->SetArcherInfo(m_PlayerInfo);
				break;
			case EPlayerJob::Magicion:
				break;
			}
		}
	}
	
}

// Called every frame
void ASelectPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASelectPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectHUD.h"
#include "CharacterSelectState.h"
#include "CharacterNameInputWidget.h"
#include "../chuchuGameInstance.h"
#include "../Player/SelectPlayerController.h"
#include "CharacterHUD.h"

void UCharacterSelectHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_StartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));
	m_BackButton = Cast<UButton>(GetWidgetFromName(TEXT("BackButton")));
	 
	m_KnightState = Cast<UCharacterSelectState>(GetWidgetFromName(TEXT("UI_SelectState_Knight")));
	m_ArcherState = Cast<UCharacterSelectState>(GetWidgetFromName(TEXT("UI_SelectState_Archer")));
	m_NameInput = Cast<UCharacterNameInputWidget>(GetWidgetFromName(TEXT("UI_CharacterNameInput")));

	m_StartButton->OnClicked.AddDynamic(this, &UCharacterSelectHUD::StartButtonClick);
	m_BackButton->OnClicked.AddDynamic(this, &UCharacterSelectHUD::BackButtonClick);

	//// ĳ���� �̹��� ���� �ε�
	UMaterialInterface* KnightMaterial = LoadObject<UMaterialInterface>(nullptr,
		TEXT("MaterialInstanceConstant'/Game/UI/MTUIRenderTargetDepthClear_Inst.MTUIRenderTargetDepthClear_Inst'"));

	m_KnightState->SetImageMaterial(KnightMaterial);

	//archer
	UMaterialInterface* ArcherMaterial = LoadObject<UMaterialInterface>(nullptr,
		TEXT("MaterialInstanceConstant'/Game/UI/MTUIRenderTargetDepthClear_Inst_Archer.MTUIRenderTargetDepthClear_Inst_Archer'"));

	m_ArcherState->SetImageMaterial(ArcherMaterial);
}

void UCharacterSelectHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCharacterSelectHUD::StartButtonClick()
{
	FString	InputName = m_NameInput->GetInputText(); //�Է��� �̸�

	// �÷��̾� �̸� �ڱ�
	UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance()); //�ν��Ͻ��� ����
	ASelectPlayerController* Controller = Cast< ASelectPlayerController>(GetWorld()->GetFirstPlayerController());
	const EPlayerJob JobName = Controller->GetSelectJob(); //�������ϱ�
	
	switch (JobName)
	{
	case EPlayerJob::Knight:
		//SetPlayerInfoName
		GameInst->SetPlayerInfoName(TEXT("Knight"), InputName);
		break;
	case EPlayerJob::Archer:
		GameInst->SetPlayerInfoName(TEXT("Archer"), InputName);
		break;
	case EPlayerJob::Magicion:
		break;
	}
	
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Main"));

	//ui_characterselecthud���� startbutton �� behavior���� Is Enabled�� üũ�� �����ϸ� ��ư�� ��Ȱ��ȭ �ǰ�
	//ĳ���͸� �����Ͽ� ����� �� ��ư�� Ȱ��ȭ �� �� �ֵ��� �� ��
}

void UCharacterSelectHUD::BackButtonClick()
{
	//UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(),
	//	EQuitPreference::Quit, true);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Start"));
}


void UCharacterSelectHUD::SetKnightStateVisibility(ESlateVisibility Visible)
{
	m_KnightState->SetVisibility(Visible);
}

void UCharacterSelectHUD::SetArcherStateVisibility(ESlateVisibility Visible)
{
	m_ArcherState->SetVisibility(Visible);
}

void UCharacterSelectHUD::SetInputNameVisibility(ESlateVisibility Visible)
{
	// (bp)behavior�� visibliity �� collapsed�� �Ǿ� �־ ó���� �Ⱥ��̴ٰ�
	// ���콺�� �����ų� � Ʈ���ſ� visible�� �־� ���������� 
	m_NameInput->SetVisibility(Visible);
}

void UCharacterSelectHUD::SetKnightInfo(const FPlayerInfo& Info)
{
	m_KnightState->SetNameText(Info.Name);
	m_KnightState->SetJobText(Info.Job);
	m_KnightState->SetAttackText(Info.Attack);
	m_KnightState->SetArmorText(Info.Armor);
	m_KnightState->SetHPText(Info.HP, Info.HPMax);
	m_KnightState->SetMPText(Info.MP, Info.MPMax);
}

void UCharacterSelectHUD::SetArcherInfo(const FPlayerInfo& Info)
{
	m_ArcherState->SetNameText(Info.Name);
	m_ArcherState->SetJobText(Info.Job);
	m_ArcherState->SetAttackText(Info.Attack);
	m_ArcherState->SetArmorText(Info.Armor);
	m_ArcherState->SetHPText(Info.HP, Info.HPMax);
	m_ArcherState->SetMPText(Info.MP, Info.MPMax);
}


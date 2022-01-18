// Fill out your copyright notice in the Description page of Project Settings.


#include "StartWidget.h"
#include "CharacterNameInputWidget.h"
#include "../chuchuGameInstance.h"
#include "../Player/SelectPlayerController.h"
#include "CharacterHUD.h"

void UStartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_StartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartGameButton")));
	m_QuitButton = Cast<UButton>(GetWidgetFromName(TEXT("QuitGameButton")));
	m_NameInput = Cast<UCharacterNameInputWidget>(GetWidgetFromName(TEXT("CharacterNameInput")));

	m_StartButton->OnClicked.AddDynamic(this, &UStartWidget::StartButton);
	m_QuitButton->OnClicked.AddDynamic(this, &UStartWidget::QuitButton);

	m_StartButton->OnHovered.AddDynamic(this, &UStartWidget::StartButtonHOvered);
	m_StartButton->OnHovered.AddDynamic(this, &UStartWidget::QuitButtonHOvered);

	// �ִϸ��̼��� ã�´�.
	//BindingAnimations();
}

void UStartWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}



void UStartWidget::StartButton()
{
	FString	InputName = m_NameInput->GetInputText(); //�Է��� �̸�

	// �÷��̾� �̸� �ڱ�
	UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance()); //�ν��Ͻ��� ����
	ASelectPlayerController* Controller = Cast< ASelectPlayerController>(GetWorld()->GetFirstPlayerController());
	GameInst->SetPlayerInfoName(TEXT("Knight"), InputName);

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Main"));

	//ui_characterselecthud���� startbutton �� behavior���� Is Enabled�� üũ�� �����ϸ� ��ư�� ��Ȱ��ȭ �ǰ�
	//ĳ���͸� �����Ͽ� ����� �� ��ư�� Ȱ��ȭ �� �� �ֵ��� �� ��
}

void UStartWidget::QuitButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(),EQuitPreference::Quit, true);
}

//34
void UStartWidget::StartButtonHOvered()
{
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Main")); //���� ����(���ξ�)�� ������ , ���� ��ȯ
	//PlayAnimation(m_StartButtonMouseOn); //�����ִ� �����Ѱ��� ������ �ִϰ� ����� ��
	LOG(TEXT("StartButtonAnim MouseOn"));
}

void UStartWidget::QuitButtonHOvered()
{
	//UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(),
	//	EQuitPreference::Quit, true); //���� ����
}


/*
void UStartWidget::BindingAnimations()
{
	//�ִϸ��̼� ã�� �� ����
	UProperty* Prop = GetClass()->PropertyLink;

	while (Prop != nullptr)
	{
		if (Prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop);

			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass()) //�����ִ��ϰ�쿡�� �������
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);

				UWidgetAnimation* Anim = Cast<UWidgetAnimation>(Obj); //�����ִϷ� ��ȯ

				if (Anim != nullptr)
				{
					if (Anim->MovieScene->GetFName() == "StartButtonMouseOn")
					{
						m_StartButtonMouseOn = Anim;
						LOG(TEXT("StartButtonAnim"));
					}
				}
			}
		}
		//��ũ�帮��Ʈ �������� �Ǿ��־ ��������
		Prop = Prop->PropertyLinkNext;
	}
}
*/
// Fill out your copyright notice in the Description page of Project Settings.


#include "StartWidget.h"


void UStartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//��ư���� ������ ��
	m_StartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));
	m_QuitButton = Cast<UButton>(GetWidgetFromName(TEXT("QuitButton")));

	m_StartButton->OnClicked.AddDynamic(this, &UStartWidget::StartButton); //��������Ʈ ��ư�� �������� startbutton�Լ��� ȣ��
	m_QuitButton->OnClicked.AddDynamic(this, &UStartWidget::QuitButton);

	m_StartButton->OnHovered.AddDynamic(this, &UStartWidget::StartButtonHOvered); //���콺�� �ö� �� �Լ��� �Ҹ�
	m_StartButton->OnHovered.AddDynamic(this, &UStartWidget::QuitButtonHOvered);

	// �ִϸ��̼��� ã�´�.
	BindingAnimations();
}

void UStartWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

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

void UStartWidget::StartButton()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("CharacterSelect"));
}

void UStartWidget::QuitButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(),
		EQuitPreference::Quit, true);
}

void UStartWidget::StartButtonHOvered()
{
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Main")); //���� ����(���ξ�)�� ������ , ���� ��ȯ
	PlayAnimation(m_StartButtonMouseOn); //�����ִ� �����Ѱ��� ������ �ִϰ� ����� ��
	LOG(TEXT("StartButtonAnim MouseOn"));
}

void UStartWidget::QuitButtonHOvered()
{
	//UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(),
	//	EQuitPreference::Quit, true); //���� ����
}
//34
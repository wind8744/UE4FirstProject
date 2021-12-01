// Fill out your copyright notice in the Description page of Project Settings.


#include "StartWidget.h"


void UStartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//버튼들을 가지고 옴
	m_StartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));
	m_QuitButton = Cast<UButton>(GetWidgetFromName(TEXT("QuitButton")));

	m_StartButton->OnClicked.AddDynamic(this, &UStartWidget::StartButton); //델리게이트 버튼을 눌렀을때 startbutton함수가 호출
	m_QuitButton->OnClicked.AddDynamic(this, &UStartWidget::QuitButton);

	m_StartButton->OnHovered.AddDynamic(this, &UStartWidget::StartButtonHOvered); //마우스가 올라갈 때 함수가 불림
	m_StartButton->OnHovered.AddDynamic(this, &UStartWidget::QuitButtonHOvered);

	// 애니메이션을 찾는다.
	BindingAnimations();
}

void UStartWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UStartWidget::BindingAnimations()
{
	//애니메이션 찾을 수 있음
	UProperty* Prop = GetClass()->PropertyLink; 

	while (Prop != nullptr)
	{
		if (Prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop); 

			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass()) //위젯애니일경우에만 가지고옴
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);

				UWidgetAnimation* Anim = Cast<UWidgetAnimation>(Obj); //위젯애니로 변환

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
		//링크드리스트 형식으로 되어있어서 다음으로
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
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Main")); //메인 레벨(메인씬)을 열어줌 , 레벨 전환
	PlayAnimation(m_StartButtonMouseOn); //위젯애니 선언한것을 넣으면 애니가 재생이 됨
	LOG(TEXT("StartButtonAnim MouseOn"));
}

void UStartWidget::QuitButtonHOvered()
{
	//UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(),
	//	EQuitPreference::Quit, true); //게임 종료
}
//34
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

	// 애니메이션을 찾는다.
	//BindingAnimations();
}

void UStartWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}



void UStartWidget::StartButton()
{
	FString	InputName = m_NameInput->GetInputText(); //입력한 이름

	// 플레이어 이름 박기
	UchuchuGameInstance* GameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance()); //인스턴스에 저장
	ASelectPlayerController* Controller = Cast< ASelectPlayerController>(GetWorld()->GetFirstPlayerController());
	GameInst->SetPlayerInfoName(TEXT("Knight"), InputName);

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Main"));

	//ui_characterselecthud에서 startbutton 의 behavior에서 Is Enabled에 체크를 해제하면 버튼이 비활성화 되고
	//캐릭터를 선택하여 골랐을 때 버튼이 활성화 될 수 있도록 할 것
}

void UStartWidget::QuitButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(),EQuitPreference::Quit, true);
}

//34
void UStartWidget::StartButtonHOvered()
{
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Main")); //메인 레벨(메인씬)을 열어줌 , 레벨 전환
	//PlayAnimation(m_StartButtonMouseOn); //위젯애니 선언한것을 넣으면 애니가 재생이 됨
	LOG(TEXT("StartButtonAnim MouseOn"));
}

void UStartWidget::QuitButtonHOvered()
{
	//UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(),
	//	EQuitPreference::Quit, true); //게임 종료
}


/*
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
*/
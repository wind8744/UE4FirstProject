// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestWidget.h"
#include "QuestListData.h"
#include "QuestListWidget.h"
#include "QuestDescWidget.h"
#include "MainHUD.h"
#include "../Player/PlayerCharacter.h"
#include "../UIItem/Inventory.h"
#include "../chuchuGameModeBase.h"
#include "../chuchuGameInstance.h"

void UQuestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_QuestList = Cast<UListView>(GetWidgetFromName(TEXT("ListView_63")));
	m_QuestDesc = Cast<UQuestDescWidget>(GetWidgetFromName(TEXT("UI_QuesDesc")));
	m_CloseButton = Cast<UButton>(GetWidgetFromName(TEXT("CloseButton")));

	m_CloseButton->OnClicked.AddDynamic(this, &UQuestWidget::CloseButtonClick);
	m_QuestList->OnItemClicked().AddUObject(this, &UQuestWidget::QuestClick);
	m_QuestList->OnItemSelectionChanged().AddUObject(this, &UQuestWidget::QuestSelect);
	m_QuestList->OnItemIsHoveredChanged().AddUObject(this, &UQuestWidget::QuestMouseOn);

	m_CurRenderQuest = nullptr;
	m_PrevSelect = nullptr;

	UchuchuGameInstance* ChuInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());
	if (ChuInst)
	{
		const TMap<FString, FQuestData>& QuestData = ChuInst->GetQuestData();
		int32 Index = 0;
		for (auto& Data : QuestData)
		{
			const FQuestTableInfo* Info = ChuInst->FindQuestTableInfo(Data.Key); //키값으로 퀘스트 정보 찾아옴

			if (Info)
			{
				m_QuestArray.Add(Data.Value); //퀘스트 정보 넣기
				int32 QuestIdx = m_QuestArray.Num() - 1;

				for (auto& QuestInfo : Info->InfoArray) //퀘스트하나에 여러 조건들이 있을 수 있으므로 
				{
					m_QuestListArray[(uint8)QuestInfo.Type].Add(&m_QuestArray[QuestIdx]);
				}

				//하나 생성해서 데이터 세팅 해주고 퀘스트 리스트에 넣어줌
				UQuestListData* questListData = NewObject<UQuestListData>(this, UQuestListData::StaticClass());
				questListData->SetIndex(Index);
				questListData->SetQuestName(Info->Name);

				m_QuestList->AddItem(questListData);

				++Index;
			}

		}
	}
}

void UQuestWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UQuestWidget::CloseButtonClick()
{
	if (GetVisibility() == ESlateVisibility::Collapsed) //인벤토리가 가려져있을때 호출되면
		SetVisibility(ESlateVisibility::SelfHitTestInvisible); //인벤토리가 보이도록

	else
	{
		SetVisibility(ESlateVisibility::Collapsed);

		//m_Equipclass->CloseEquipUI();

		APlayerController* PController = GetWorld()->GetFirstPlayerController();
		PController->SetInputMode(FInputModeGameOnly()); // 커서 없어지고 마우스 방향으로 카메라 회전
		PController->bShowMouseCursor = false;
	}
}

void UQuestWidget::CloseInvenUI()
{
	if (GetVisibility() == ESlateVisibility::Collapsed) //인벤토리가 가려져있을때 호출되면
		SetVisibility(ESlateVisibility::SelfHitTestInvisible); //인벤토리가 보이도록

	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

#include "Blueprint/WidgetBlueprintLibrary.h"
FReply UQuestWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Allows users to handle events and return information to the underlying UI layer.
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, FKey("LeftMouseButton")); //왼쪽 마우스를 누르면 이벤트가 들어옴
	//DetectDragIfPressed 이; 함수를 통해 밑의 NativeOnDragDetected함수가 호출됨

	return reply.NativeReply;
}

#include "../UI/WidgetDragDropOperation.h"
void UQuestWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UWidgetDragDropOperation* Wigddo = Cast<UWidgetDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UWidgetDragDropOperation::StaticClass()));
	//내가 만든 드래그드랍 클래스 갖고옴

	if (nullptr == Wigddo)
		return;

	Wigddo->WidgetTodrag = this;
	//옮길 위젯 (멤버변수로 저장)

	Wigddo->MouseOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	//마우스 offset (멤버변수에 저장)

	Wigddo->DefaultDragVisual = this;
	//드래그 할 때 보일 위젯 

	Wigddo->Pivot = EDragPivot::MouseDown;
	//마우스 다운을 한 지점을 기준으로

	OutOperation = Wigddo;
	//outoperation의 참ㅁ조가 널이 아니라면 native on drop이 호출

	this->RemoveFromParent(); //드래그 할 때 그 전에있던것은 삭제 
}

// 퀘스트 리스트 클릭
void UQuestWidget::QuestClick(UObject* Data)
{
	UQuestListData* Info = Cast<UQuestListData>(Data);

	if (Info)
	{
		if (m_CurRenderQuest == &m_QuestArray[Info->GetIndex()])
			return;

		m_CurRenderQuest = &m_QuestArray[Info->GetIndex()];

		FString	QuestName = m_QuestArray[Info->GetIndex()].Name;

		if (m_CurRenderQuest->Complete)
			QuestName = m_QuestArray[Info->GetIndex()].Name + TEXT("(완료)");

		m_QuestDesc->SetQuestName(QuestName);
		m_QuestDesc->SetQuestDesc(m_QuestArray[Info->GetIndex()].QuestDesc);

		m_QuestDesc->ClearList();

		int32	Index = 0;

		for (auto& DataInfo : m_QuestArray[Info->GetIndex()].CompleteArray)
		{
			m_QuestDesc->AddList(DataInfo, Index);
			++Index;
		}
	}
}

void UQuestWidget::QuestSelect(UObject* Data)
{
	UQuestListData* Item = Cast<UQuestListData>(Data);
	PrintViewport(5.f, FColor::Red, Item->GetQuestName());

	if (m_PrevSelect)
		m_PrevSelect->GetOwnerWidget()->Select(false);

	Item->GetOwnerWidget()->Select(true);

	m_PrevSelect = Item;
}

void UQuestWidget::QuestMouseOn(UObject* Data,bool IsHovered)
{
	UQuestListData* Item = Cast<UQuestListData>(Data);

	Item->GetOwnerWidget()->MouseOn(IsHovered);
}

void UQuestWidget::QuestCheck(EQuestType _type, const FString& Name)
{
	
	for (auto& Info : m_QuestListArray[(uint8)_type])
	{
		if (Info->Complete)
			continue;

		int32	CompleteCount = 0;

		for (auto& QuestData : Info->CompleteArray)
		{
			if (QuestData.Complete)
			{
				++CompleteCount;
				continue;
			}

			if (QuestData.type == _type && QuestData.DestName == Name)
			{
				++QuestData.Count;

				if (QuestData.Count == QuestData.MaxCount)
				{
					QuestData.Complete = true;
					++CompleteCount;
				}

				// 현재 퀘스트창에 보여지고 있는 퀘스트가 변경이 일어났을 경우
				// 갱신한다.
				if (m_CurRenderQuest == Info)
				{
					m_QuestDesc->SetQuestName(m_CurRenderQuest->Name);
					m_QuestDesc->SetQuestDesc(m_CurRenderQuest->QuestDesc);

					m_QuestDesc->ClearList();

					int32	CompleteIndex = 0;

					for (auto& DataInfo : m_CurRenderQuest->CompleteArray)
					{
						m_QuestDesc->AddList(DataInfo, CompleteIndex);
						++CompleteIndex;
					}
				}
			}
		}

		if (CompleteCount == Info->CompleteArray.Num())
		{
			Info->Complete = true;

			// 퀘스트 완료시 보상 처리

			for (auto& CompensationData : Info->CompensationArray)
			{
				switch (CompensationData.Type)
				{
				case EQuestCompensationType::Gold:
				{
					int32	Gold = FCString::Atoi(*CompensationData.Compensation);

					APlayerCharacter* Player = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();

					if (Player)
						Player->AddGold(Gold);
				}
				break;
				case EQuestCompensationType::Item:
				{
					UchuchuGameInstance* ChuInst = GetWorld()->GetGameInstance<UchuchuGameInstance>();

					//if (ChuInst)
					//{
					//	const FUIItemTableInfo* ItemInfo = ChuInst->FindUIItemInfo(CompensationData.Compensation);

					//	if (ItemInfo)
					//	{
					//		AchuchuGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AchuchuGameModeBase>();

					//		if (GameMode)
					//		{
					//			UMainHUD* MainHUD = GameMode->GetMainHUD();

					//			if (MainHUD)
					//			{
					//				MainHUD->GetInventory()->AddItem(ItemInfo);
					//			}
					//		}
					//	}
					//}
				}
				break;
				case EQuestCompensationType::Exp:
				{
					int32	Exp = FCString::Atoi(*CompensationData.Compensation);

					APlayerCharacter* Player = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();

					if (Player)
						Player->AddExp(Exp);
				}
				break;
				}
			}
		}
	}
}
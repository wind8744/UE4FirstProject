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

	m_QuestList = Cast<UListView>(GetWidgetFromName(TEXT("ListView1")));
	m_QuestDesc = Cast<UQuestDescWidget>(GetWidgetFromName(TEXT("UI_QuesDesc")));

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
//리스트 뷰가 클릭되었을떄
void UQuestWidget::QuestSelect(UObject* Data)
{
	UQuestListData* Item = Cast<UQuestListData>(Data);
	//PrintViewport(5.f, FColor::Red, Item->GetQuestName());

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
		FQuestDataInfo Questtest = Info->CompleteArray[(uint8)_type];
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

					if (ChuInst)
					{
						const FUIItemDataInfo* ItemInfo = ChuInst->FindUIItemInfo(CompensationData.Compensation);

						if (ItemInfo)
						{
							AchuchuGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AchuchuGameModeBase>();

							if (GameMode)
							{
								UMainHUD* MainHUD = GameMode->GetMainHUD();

								if (MainHUD)
								{
									UItemData* ItemData = NewObject<UItemData>(this, UItemData::StaticClass());
									ItemData->SetNameText(ItemInfo->m_ItemName);
									ItemData->SetIconTex(ItemInfo->m_Thumbnail);
									ItemData->SetMeshPath(ItemInfo->m_MeshPath);
									ItemData->SetItemType(EItemType::Food, ItemInfo->m_EquipType);
									ItemData->SetPickMesh(ItemInfo->m_PickMesh);
									MainHUD->GetInventory()->AddItem(ItemData);
								}
							}
						}
					}
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
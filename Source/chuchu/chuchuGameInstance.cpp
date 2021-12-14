// Fill out your copyright notice in the Description page of Project Settings.


#include "chuchuGameInstance.h"

UchuchuGameInstance::UchuchuGameInstance()
{
	// 몬스터 데이터 에셋 가져오기
	static ConstructorHelpers::FObjectFinder<UDataTable>	MonsterInfoTableAsset(TEXT("DataTable'/Game/Monster/DTMonsterInfo.DTMonsterInfo'"));

	if (MonsterInfoTableAsset.Succeeded())
		m_MonsterInfoTable = MonsterInfoTableAsset.Object;

	// 플레이어 데이터 에셋 가져오기
	static ConstructorHelpers::FObjectFinder<UDataTable>	PlayerInfoTableAsset(TEXT("DataTable'/Game/Player/DTPlayerInfo.DTPlayerInfo'"));

	if (PlayerInfoTableAsset.Succeeded())
		m_PlayerInfoTable = PlayerInfoTableAsset.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	UIItemDataAsset(TEXT("DataTable'/Game/Item/UI/DTItemDataInfo.DTItemDataInfo'"));//UIItemInfoTableAsset(TEXT("DataTable'/Game/UI/DTUIItemInfo.DTUIItemInfo'"));

	if (UIItemDataAsset.Succeeded())
		m_UIItemInfoTable = UIItemDataAsset.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	QuestDataAsset(TEXT("DataTable'/Game/UI/QuestUI/DTQuestTableInfo.DTQuestTableInfo'"));
	if (QuestDataAsset.Succeeded())
		m_QuestTableInfo = QuestDataAsset.Object;
}

void UchuchuGameInstance::Init()
{
	Super::Init();

	//ProjectilePool::GetInst()->Init(GetWorld());
	//NetworkManager::GetInst()->Init();

	// 각 퀘스트들이 완료가 되었는지 여부를 가져온다.
	FQuestData	Data;
	
	const FQuestTableInfo* Info = FindQuestTableInfo(TEXT("Quest1"));

	Data.Name = Info->Name;
	Data.QuestDesc = Info->QuestDesc;

	for (auto& CompleteData : Info->InfoArray)
	{
		FQuestDataInfo	DataInfo;

		DataInfo.type = CompleteData.Type;
		DataInfo.DestName = CompleteData.DestName;
		DataInfo.MaxCount = CompleteData.Count;
		DataInfo.Count = 0;
		DataInfo.Complete = false;

		Data.CompleteArray.Add(DataInfo);
	}

	for (auto& CompensationData : Info->CompensationArray)
	{
		FQuestCompensationInfo	DataInfo;

		DataInfo.Type = CompensationData.Type;
		DataInfo.Compensation = CompensationData.Compensation;

		Data.CompensationArray.Add(DataInfo);
	}

	Data.Complete = false;

	m_QuestComplete.Add(TEXT("Quest1"), Data);

	Info = FindQuestTableInfo(TEXT("Quest2"));

	Data.Name = Info->Name;
	Data.QuestDesc = Info->QuestDesc;
	Data.CompleteArray.RemoveAll([](FQuestDataInfo v) {return true; });
	Data.CompensationArray.RemoveAll([](FQuestCompensationInfo v) {return true; });

	for (auto& CompleteData : Info->InfoArray)
	{
		FQuestDataInfo	DataInfo;

		DataInfo.type = CompleteData.Type;
		DataInfo.DestName = CompleteData.DestName;
		DataInfo.MaxCount = CompleteData.Count;
		DataInfo.Count = 0;
		DataInfo.Complete = false;

		Data.CompleteArray.Add(DataInfo);
	}

	for (auto& CompensationData : Info->CompensationArray)
	{
		FQuestCompensationInfo	DataInfo;

		DataInfo.Type = CompensationData.Type;
		DataInfo.Compensation = CompensationData.Compensation;

		Data.CompensationArray.Add(DataInfo);
	}

	Data.Complete = false;

	m_QuestComplete.Add(TEXT("Quest2"), Data);

	//Info = FindQuestTableInfo(TEXT("Quest3"));

	//Data.Name = Info->Name;
	//Data.QuestDesc = Info->QuestDesc;
	//Data.CompleteArray.RemoveAll([](FQuestDataInfo v) {return true; });
	//Data.CompensationArray.RemoveAll([](FQuestCompensationInfo v) {return true; });

	//for (auto& CompleteData : Info->InfoArray)
	//{
	//	FQuestDataInfo	DataInfo;

	//	DataInfo.type= CompleteData.Type;
	//	DataInfo.DestName = CompleteData.DestName;
	//	DataInfo.MaxCount = CompleteData.Count;
	//	DataInfo.Count = 0;
	//	DataInfo.Complete = false;

	//	Data.CompleteArray.Add(DataInfo);
	//}

	//for (auto& CompensationData : Info->CompensationArray)
	//{
	//	FQuestCompensationInfo	DataInfo;

	//	DataInfo.Type = CompensationData.Type;
	//	DataInfo.Compensation = CompensationData.Compensation;

	//	Data.CompensationArray.Add(DataInfo);
	//}

	//Data.Complete = false;

	//m_QuestComplete.Add(TEXT("Quest3"), Data);

	//Info = FindQuestTableInfo(TEXT("Quest4"));

	//Data.Name = Info->Name;
	//Data.QuestDesc = Info->QuestDesc;
	//Data.CompleteArray.RemoveAll([](FQuestDataInfo v) {return true; });
	//Data.CompensationArray.RemoveAll([](FQuestCompensationInfo v) {return true; });

	//for (auto& CompleteData : Info->InfoArray)
	//{
	//	FQuestDataInfo	DataInfo;

	//	DataInfo.type = CompleteData.Type;
	//	DataInfo.DestName = CompleteData.DestName;
	//	DataInfo.MaxCount = CompleteData.Count;
	//	DataInfo.Count = 0;
	//	DataInfo.Complete = false;

	//	Data.CompleteArray.Add(DataInfo);
	//}

	//for (auto& CompensationData : Info->CompensationArray)
	//{
	//	FQuestCompensationInfo	DataInfo;

	//	DataInfo.Type = CompensationData.Type;
	//	DataInfo.Compensation = CompensationData.Compensation;

	//	Data.CompensationArray.Add(DataInfo);
	//}

	//Data.Complete = false;

	//m_QuestComplete.Add(TEXT("Quest4"), Data);


}

const FMonsterTableInfo* UchuchuGameInstance::FindMonsterInfo(const FString& Name)
{
	return m_MonsterInfoTable->FindRow<FMonsterTableInfo>(*Name, ""); //월드를 통해서 얻어오는 것이므로 생성자에서 쓰일 수 없다...... 
}

const FPlayerTableInfo* UchuchuGameInstance::FindPlayerInfo(const FString& Name)
{
	return m_PlayerInfoTable->FindRow<FPlayerTableInfo>(*Name, "");
}
const FUIItemDataInfo* UchuchuGameInstance::FindUIItemInfo(const FString& Name)
{
	return m_UIItemInfoTable->FindRow<FUIItemDataInfo>(*Name, "");
}

const FQuestTableInfo* UchuchuGameInstance::FindQuestTableInfo(const FString& Name)
{
	return m_QuestTableInfo->FindRow<FQuestTableInfo>(*Name, "");
}

void UchuchuGameInstance::SetPlayerInfoName(const FString& Job, const FString& Name)
{
	FPlayerTableInfo* SelectJobINfo = m_PlayerInfoTable->FindRow<FPlayerTableInfo>(*Job, "");
	SelectJobINfo->Name = Name;
	m_SelectName = Name;
}

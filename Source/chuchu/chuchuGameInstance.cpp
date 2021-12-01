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
}

void UchuchuGameInstance::Init()
{
	Super::Init();
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

void UchuchuGameInstance::SetPlayerInfoName(const FString& Job, const FString& Name)
{
	FPlayerTableInfo* SelectJobINfo = m_PlayerInfoTable->FindRow<FPlayerTableInfo>(*Job, "");
	SelectJobINfo->Name = Name;
	m_SelectName = Name;
}

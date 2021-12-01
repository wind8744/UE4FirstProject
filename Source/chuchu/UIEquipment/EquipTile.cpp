// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipTile.h"
#include "../chuchuGameInstance.h"
#include "../UIItem/ItemData.h"
#include "../UIItem/Inventory.h"
#include "../Player/GhostLady.h"

void UEquipTile::NativeConstruct()
{
	Super::NativeConstruct();

	//블프의 위젯과 멤버변수를 연결해줌
	m_IconTex = Cast<UImage>(GetWidgetFromName("EquipIcon"));
	m_EqBorder = Cast<UBorder>(GetWidgetFromName("Border_61"));

	m_EqBorder->OnMouseButtonDownEvent.BindDynamic(this, &UEquipTile::OnMouseButtonDoubleClickBorder);  
	// 아이콘을 자식으로 두고 이벤트 바인드 ,
	// 함수명은 자유롭게 해도 되지만, 매개변수를 정확히 써줘야 컴파일이 된다.

	m_IsEquip = false;
}

void UEquipTile::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

FEventReply UEquipTile::OnMouseButtonDoubleClickBorder(FGeometry InMyGeometry, const FPointerEvent& InMouseEvent)
{
	if (m_IsEquip)
	{
		UchuchuGameInstance* gameInst = Cast<UchuchuGameInstance>(GetWorld()->GetGameInstance());
		const FUIItemDataInfo* ItemInfo = gameInst->FindUIItemInfo(m_ItemName);
		if (ItemInfo)
		{
			UItemData* ItemData1 = NewObject<UItemData>(this, UItemData::StaticClass());
			ItemData1->SetNameText(ItemInfo->m_ItemName);
			ItemData1->SetIconTex(ItemInfo->m_Thumbnail);
			ItemData1->SetMeshPath(ItemInfo->m_MeshPath);
			ItemData1->SetItemDesc(ItemInfo->m_ItemDesc);
			ItemData1->SetItemType(EItemType::Equip, ItemInfo->m_EquipType);
			ItemData1->SetPickMesh(ItemInfo->m_PickMesh);
			m_Inventoryclass->AddItem(ItemData1);

			//장비 벗는것 m_ItemOldName
			APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
			AGhostLady* ghostchar = Cast<AGhostLady>(Player); //원래는 캐릭터 공통이므로 playercharacter 에 함수 만드는 것이 낫다..
			ghostchar->RemoveItem(ItemInfo->m_EquipType);
			
		}
	}

	m_IsEquip = false;
	UTexture2D* iconTEx = LoadObject<UTexture2D>(nullptr, (TEXT("Texture2D'/Game/Classic_RPG_GUI/frame_backgrounds/helm_background.helm_background'")));
	SetIconTex(iconTEx);

	return FEventReply::FEventReply();
}

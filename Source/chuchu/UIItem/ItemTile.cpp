
#include "ItemTile.h"
#include "ItemData.h"

void UItemTile::NativeConstruct()
{
	Super::NativeConstruct();
	//블프의 위젯과 멤버변수를 연결해줌
	m_IconImage = Cast<UImage>(GetWidgetFromName("Icon"));
	m_IconCount = Cast<UTextBlock>(GetWidgetFromName("Count"));
	m_IsFood = false;
}

void UItemTile::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (m_IsFood && nullptr != m_ItemData)
	{
		int32 ret = m_ItemData->GetIndex();
		m_IconCount->SetText(FText::FromString(FString::FromInt(ret+1)));
	}
}

void UItemTile::SetData(UObject* Data)
{
	//UItemData* itemdata = Cast<UItemData>(Data); //오브젝트 데이타 클래스 캐스트
	m_ItemData = Cast<UItemData>(Data); //오브젝트 데이타 클래스 캐스트
	UTexture2D* iconTEx = m_ItemData->GetIconTex();

	if (iconTEx) //타일에 들어온 아이템 이미지 붙이기
	{
		m_IconImage->SetBrushFromTexture(iconTEx);
	}

	if (m_ItemData->GetItemType() == EItemType::Food)
	{
		m_IsFood = true;
		int32 ret = m_ItemData->GetIndex();
		m_IconCount->SetText(FText::FromString(FString::FromInt(ret)));
	}
}

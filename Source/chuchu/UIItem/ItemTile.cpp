
#include "ItemTile.h"
#include "ItemData.h"

void UItemTile::NativeConstruct()
{
	Super::NativeConstruct();
	//������ ������ ��������� ��������
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
	//UItemData* itemdata = Cast<UItemData>(Data); //������Ʈ ����Ÿ Ŭ���� ĳ��Ʈ
	m_ItemData = Cast<UItemData>(Data); //������Ʈ ����Ÿ Ŭ���� ĳ��Ʈ
	UTexture2D* iconTEx = m_ItemData->GetIconTex();

	if (iconTEx) //Ÿ�Ͽ� ���� ������ �̹��� ���̱�
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

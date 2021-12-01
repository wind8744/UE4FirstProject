
#include "ItemTile.h"
#include "ItemData.h"

void UItemTile::NativeConstruct()
{
	Super::NativeConstruct();
	//������ ������ ��������� ��������
	m_IconImage = Cast<UImage>(GetWidgetFromName("Icon"));
}

void UItemTile::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UItemTile::SetData(UObject* Data)
{
	UItemData* itemdata = Cast<UItemData>(Data); //������Ʈ ����Ÿ Ŭ���� ĳ��Ʈ

	//UTexture2D* icontex = LoadObject<UTexture2D>(nullptr, *itemdata->GetIconPath());
	UTexture2D* iconTEx = itemdata->GetIconTex();

	if (iconTEx) //Ÿ�Ͽ� ���� ������ �̹��� ���̱�
	{
		m_IconImage->SetBrushFromTexture(iconTEx);
	}
}

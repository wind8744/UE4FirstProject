
#include "ItemTile.h"
#include "ItemData.h"

void UItemTile::NativeConstruct()
{
	Super::NativeConstruct();
	//블프의 위젯과 멤버변수를 연결해줌
	m_IconImage = Cast<UImage>(GetWidgetFromName("Icon"));
}

void UItemTile::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UItemTile::SetData(UObject* Data)
{
	UItemData* itemdata = Cast<UItemData>(Data); //오브젝트 데이타 클래스 캐스트

	//UTexture2D* icontex = LoadObject<UTexture2D>(nullptr, *itemdata->GetIconPath());
	UTexture2D* iconTEx = itemdata->GetIconTex();

	if (iconTEx) //타일에 들어온 아이템 이미지 붙이기
	{
		m_IconImage->SetBrushFromTexture(iconTEx);
	}
}

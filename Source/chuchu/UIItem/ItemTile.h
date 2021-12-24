#pragma once

#include "../GameInfo.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "ItemTile.generated.h"

/**
 * 1. �κ��丮 ui Ŭ����
 * 2. �κ��丮 ui���� ǥ���� ������ui Ŭ����
 * 3. �κ��丮 ������ ui������ ��¥ item data
 */
UCLASS()
class CHUCHU_API UItemTile : public UUserWidget //2. �κ��丮 �� ������ ui 
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UImage* m_IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_IconCount;

	class UItemData* m_ItemData;
	bool m_IsFood;

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UFUNCTION(BlueprintCallable)
		void SetData(UObject* Data);
};

#pragma once

#include "../GameInfo.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "ItemTile.generated.h"

/**
 * 1. 인벤토리 ui 클래스
 * 2. 인벤토리 ui안의 표현될 아이템ui 클래스
 * 3. 인벤토리 아이템 ui내부의 진짜 item data
 */
UCLASS()
class CHUCHU_API UItemTile : public UUserWidget //2. 인벤토리 내 아이템 ui 
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UImage* m_IconImage;

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UFUNCTION(BlueprintCallable)
		void SetData(UObject* Data);
};

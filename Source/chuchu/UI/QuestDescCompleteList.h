// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "QuestDescCompleteList.generated.h"

/**
 * 
 */
UCLASS()
// 다중삼속
// 블루프린트로 이벤트 연결x c++로 하는 방법 ( 인강 11/5 29분 )
class CHUCHU_API UQuestDescCompleteList : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_TypeText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_DescText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_CountText;

	int32 m_Index;

public:
	int32 GetIndex() const { return m_Index; }
	void SetIndex(int32 Index) { m_Index = Index; }

	void SetTypeText(const FString& Text) { m_TypeText->SetText(FText::FromString(Text)); }
	void SetDescText(const FString& Text) { m_TypeText->SetText(FText::FromString(Text)); }
	void SetCountText(const FString& Text) { m_TypeText->SetText(FText::FromString(Text)); }

public:
	//다중상속으로 만들어진 함수
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject);
	virtual void NativeOnItemSelectionChanged(bool bIsSelected);

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

};

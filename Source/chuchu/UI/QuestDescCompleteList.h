// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "QuestDescCompleteList.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API UQuestDescCompleteList : public UUserWidget
{
	GENERATED_BODY()
protected:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		//UButton* m_StartButton;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		//UButton* m_QuitButton;

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
};

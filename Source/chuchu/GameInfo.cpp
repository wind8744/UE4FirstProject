// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInfo.h"

DEFINE_LOG_CATEGORY(chuchu);

//시간, 색상, text를 받아서 에디터에 출력
void PrintViewport(float Time, const FColor& Color, const FString& Text)
{
	//gegine이라고 전역변수로 잡혀 있음 이 함수를 통해서 에디터에 출력
	//-1은 아무렇게나 출력
	GEngine->AddOnScreenDebugMessage(-1, Time, Color, Text);
}

FString GetEnumToString(EEquipType value)
{
    const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEquipType"), true);
    if (!enumPtr)
    {
        return FString("Invalid");
    }
    return enumPtr->GetEnumName((int32)value);
}
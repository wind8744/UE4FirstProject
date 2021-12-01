// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInfo.h"

DEFINE_LOG_CATEGORY(chuchu);

//�ð�, ����, text�� �޾Ƽ� �����Ϳ� ���
void PrintViewport(float Time, const FColor& Color, const FString& Text)
{
	//gegine�̶�� ���������� ���� ���� �� �Լ��� ���ؼ� �����Ϳ� ���
	//-1�� �ƹ����Գ� ���
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
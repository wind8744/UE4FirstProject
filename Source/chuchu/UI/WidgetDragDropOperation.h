// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/DragDropOperation.h"
#include "WidgetDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API UWidgetDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

protected:

public:
	class UUserWidget* WidgetTodrag;//UWidget* WidgetTodrag;
	FVector2D MouseOffset;
	
};

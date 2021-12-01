// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItemDataTile.generated.h"

/**
 * 
 */
UCLASS()
class CHUCHU_API UInventoryItemDataTile : public UObject
{
	GENERATED_BODY()
public:
	UInventoryItemDataTile();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString	m_NameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString	m_IconPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32	m_Index;

	UTexture2D* m_IconTexture;

public:
	void SetIconTexture(UTexture2D* IconTex)
	{
		m_IconTexture = IconTex;
	}

	void SetNameText(const FString& NameText)
	{
		m_NameText = NameText;
	}

	UTexture2D* GetIconTexture()
	{
		return m_IconTexture;
	}

	FString GetNameText()
	{
		return m_NameText;
	}

	FString GetIconPath()
	{
		return m_IconPath;
	}

	int32 GetIndex()
	{
		return m_Index;
	}

public:
	void SetIconPath(const FString& Path)
	{
		m_IconPath = Path;
	}

	void SetIndex(int32 Index)
	{
		m_Index = Index;
	}
};

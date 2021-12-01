// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "AssetPathMain.generated.h"

/**
 * 
 */
UCLASS(config=MainAsset)
class CHUCHU_API UAssetPathMain : public UObject
{
	GENERATED_BODY()
	
public:
	UAssetPathMain();

private:
	UPROPERTY(Config)
		TArray<FAssetPathInfo>	MaterialPathArray; //경로랑 이름이 필요하므로 게임인포헤더에 구조체 만들어놓음

	UPROPERTY(Config)
		TArray<FAssetPathInfo>	ParticlePathArray;

	UPROPERTY(Config)
		TArray<FAssetPathInfo>	SoundPathArray;

	UPROPERTY(Config)
		TArray<FAssetPathInfo>	MeshPathArray;

	TMap<FString, FSoftObjectPath> m_mapMeshPath;
	TMap<FString, FSoftObjectPath>	m_mapMaterialPath;
	TMap<FString, FSoftObjectPath>	m_mapParticlePath;
	TMap<FString, FSoftObjectPath>	m_mapSoundPath;

public:
	void ConvertPath();
	const FSoftObjectPath* FindPath(const FString& Name);
	const FSoftObjectPath* FindSoundPath(const FString& Name);
	const FSoftObjectPath* FindParticlePath(const FString& Name);
	const FSoftObjectPath* FindMeshPath(const FString& Name);
};

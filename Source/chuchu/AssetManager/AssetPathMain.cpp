// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetPathMain.h"

UAssetPathMain::UAssetPathMain()
{
}

void UAssetPathMain::ConvertPath()
{
	for (auto& result : MaterialPathArray)
	{
		m_mapMaterialPath.Add(result.Name, result.Path);
		LOG(TEXT("Name : %s Path : %s"), *result.Name, *result.Path.ToString());
	}

	for (auto& result : ParticlePathArray)
	{
		m_mapParticlePath.Add(result.Name, result.Path);
		LOG(TEXT("Name : %s Path : %s"), *result.Name, *result.Path.ToString());
	}

	for (auto& result : SoundPathArray)
	{
		m_mapSoundPath.Add(result.Name, result.Path);
		LOG(TEXT("Name : %s Path : %s"), *result.Name, *result.Path.ToString());
	}


	for (auto& result : MeshPathArray)
	{
		m_mapMeshPath.Add(result.Name, result.Path);
		LOG(TEXT("Name : %s Path : %s"), *result.Name, *result.Path.ToString());
	}
}

const FSoftObjectPath* UAssetPathMain::FindPath(const FString& Name)
{
	return m_mapMaterialPath.Find(Name); 
	//배열은 첫번재부터 찰례대로 찾으므로 오래걸리므로 
	//키값으로 찾는 맵에 저장
	//원하는 곳에서 리턴받아서 사용하면 된다.
}

const FSoftObjectPath* UAssetPathMain::FindSoundPath(const FString& Name)
{
	return m_mapSoundPath.Find(Name);
}

const FSoftObjectPath* UAssetPathMain::FindParticlePath(const FString& Name)
{
	return m_mapParticlePath.Find(Name);
}
const FSoftObjectPath* UAssetPathMain::FindMeshPath(const FString& Name)
{
	return m_mapMeshPath.Find(Name);
}

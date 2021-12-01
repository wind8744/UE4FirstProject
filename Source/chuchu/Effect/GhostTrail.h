// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GhostTrail.generated.h"

UENUM(BlueprintType)
enum class EGhostTrailType : uint8
{
	LifeSpan,
	Fade
};

UCLASS()
class CHUCHU_API AGhostTrail : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGhostTrail();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UPoseableMeshComponent* m_Mesh;

	UMaterialInterface* m_GhostMaterial; //원본 메테리얼 저장
	TArray<UMaterialInstanceDynamic*>	m_GhostMaterialArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EGhostTrailType	m_GhostType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	m_LifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	m_FadeTime;

	float	m_FadeTimeAcc;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetMesh(const FString& Path);
	void SetMesh(USkeletalMesh* Mesh); //set 메쉬 잔상을 만들때 스켈레톤 메쉬 
	void CopyAnimation(USkeletalMeshComponent* Com);
};

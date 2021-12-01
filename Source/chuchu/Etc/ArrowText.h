// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "ArrowText.generated.h"

UCLASS()
class CHUCHU_API AArrowText : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AArrowText();

protected:
	// ������ ������Ʈ -> �簢�� �ϳ��� ���
	// �׻� ī�޶� �ٶ󺸰� �־� ��� ���� �簢������ ���δ� (uió��)
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UMaterialBillboardComponent* m_BillBoard;

	// 3���� ������ �ؽ��� ���( umz ����ص� ���� ���� )
	// ȸ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* m_Text;


	/*
	* ���׸��󿡼� ������ �����ϰ� ������ 
	* -> ���� ��� Translucency�� ���� �� �����Ͽ��� Translucency �� Screen Space Reflect üũ !
	*/
	UMaterialInstanceDynamic* m_Material;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

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
	// 빌보드 컴포넌트 -> 사각형 하나를 출력
	// 항상 카메라를 바라보고 있어 어디서 보든 사각형으로 보인다 (ui처럼)
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UMaterialBillboardComponent* m_BillBoard;

	// 3차원 공간에 텍스쳐 출력( umz 사용해도 문제 없음 )
	// 회전은 되지 않음
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* m_Text;


	/*
	* 메테리얼에서 반투명 적용하고 싶을때 
	* -> 블랜드 모드 Translucency로 변경 후 디테일에서 Translucency 의 Screen Space Reflect 체크 !
	*/
	UMaterialInstanceDynamic* m_Material;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

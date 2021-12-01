// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemList.h"
#include "InventoryItemDataList.h"

void UInventoryItemList::NativeConstruct()
{
	Super::NativeConstruct();

	m_IconImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemIcon")));
	m_NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameText")));
}

void UInventoryItemList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryItemList::SetData(UObject* Data)
{
	/*
	* #42������
	inventorylist.cpp ����
	AddItem �Լ��� �Ҹ� �� �Ʊ� �������Ҵ�
		  
		//	�̺�Ʈ���� on list item object �̺�Ʈ�� �߰��� �� �ִµ�
		//	����Ʈ���� �������� �߰��� �� �� �� �������̽��� �ڵ����� ȣ��ǰ� ������ش�.
		//	��Ǫ�� �߰���
		//
		// On List Item Object Set �̺�Ʈ �Լ��� ȣ��ȴ�.

	������ ����� ���� ������ ������ cpp ������ ȣ���� �� �ִ�.
	�������� �� �̺�Ʈ�� setdata�Լ��� ����(��������)�ؼ�
	setdata�Լ��� cpp���� �Ҹ����� �� ����

	cpp���� additem�Լ��� ������ �� ��
	on List item object set�̺�Ʈ �Լ��� ȣ���� �� ���̰�
	List item object�� ���� ������ ������ ����
	�� �����ʹ� ����� setdata�Լ��� data ������ ���ð���
	*/

	UInventoryItemDataList* InvenData = Cast<UInventoryItemDataList>(Data);

	m_NameText->SetText(FText::FromString(InvenData->GetNameText())); //�̸� �ְ�

	UTexture2D* IconTexture = LoadObject<UTexture2D>(nullptr,
		*InvenData->GetIconPath());

	if (IconTexture)
		m_IconImage->SetBrushFromTexture(IconTexture);  //�̹��� �ְ�
}

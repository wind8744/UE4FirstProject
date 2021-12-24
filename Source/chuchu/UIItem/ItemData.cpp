// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData.h"
#include "Inventory.h"
#include "../UIEquipment/Equipment.h"
#include "../UIEquipment/EquipTile.h"
#include "../Player/PlayerCharacter.h"
#include "../Player/GhostLady.h"
#include "../chuchuGameInstance.h"

UItemData::UItemData()
{
	m_Index = 0;
}

void UItemData::Use(class APlayerCharacter* character) //(AGhostLady* character)
{

	if (m_ItemType == EItemType::Food)
	{
		AGhostLady* ghostchar = Cast<AGhostLady>(character);
		ghostchar->UseItem();
	}
	else if(m_ItemType == EItemType::Equip)
	{
		switch (m_EquipType)
		{
			case EEquipType::HAIR:
			{
				AGhostLady* ghostchar = Cast<AGhostLady>(character); //원래는 캐릭터 공통이므로 playercharacter 에 함수 만드는 것이 낫다..
				ghostchar->EquipItem(m_EquipType, m_MeshPath);

				if (OwningInventory)
				{
					UEquipTile* equipTile = OwningInventory->GetEquipClass()->GetEquipTileArray()[(int)EEquipType::HAIR];
					equipTile->SetIconTex(m_IconTex);
					equipTile->SetItemName(m_ItemName);
					equipTile->SetIsEquip(true);
				}
				break;
			}
			case EEquipType::TOPBODY:
			{
				AGhostLady* ghostchar = Cast<AGhostLady>(character);
				ghostchar->EquipItem(m_EquipType, m_MeshPath);

				if (OwningInventory)
				{
					UEquipTile* equipTile = OwningInventory->GetEquipClass()->GetEquipTileArray()[(int)EEquipType::TOPBODY];
					equipTile->SetIconTex(m_IconTex);
					equipTile->SetItemName(m_ItemName);
					equipTile->SetIsEquip(true);
				}

				break;
			}
			case EEquipType::BOTBODY:
			{
				AGhostLady* ghostchar = Cast<AGhostLady>(character);
				ghostchar->EquipItem(m_EquipType, m_MeshPath);

				if (OwningInventory)
				{
					UEquipTile* equipTile = OwningInventory->GetEquipClass()->GetEquipTileArray()[(int)EEquipType::BOTBODY];
					equipTile->SetIconTex(m_IconTex);
					equipTile->SetItemName(m_ItemName);
					equipTile->SetIsEquip(true);
				}

				break;
			}
			case EEquipType::HAND:
			{
				AGhostLady* ghostchar = Cast<AGhostLady>(character);
				ghostchar->EquipItem(m_EquipType, m_MeshPath);

				if (OwningInventory)
				{
					UEquipTile* equipTile = OwningInventory->GetEquipClass()->GetEquipTileArray()[(int)EEquipType::HAND];
					equipTile->SetIconTex(m_IconTex);
					equipTile->SetItemName(m_ItemName);
					equipTile->SetIsEquip(true);
				}

				break;
			}
			case EEquipType::BOOTS:
			{
				AGhostLady* ghostchar = Cast<AGhostLady>(character);
				ghostchar->EquipItem(m_EquipType, m_MeshPath);

				if (OwningInventory)
				{
					UEquipTile* equipTile = OwningInventory->GetEquipClass()->GetEquipTileArray()[(int)EEquipType::BOOTS];
					equipTile->SetIconTex(m_IconTex);
					equipTile->SetItemName(m_ItemName);
					equipTile->SetIsEquip(true);
				}

				break;
			}
			case EEquipType::WEAPON:
			{
				AGhostLady* ghostchar = Cast<AGhostLady>(character);
				ghostchar->EquipItem(m_EquipType, m_MeshPath);

				if (OwningInventory)
				{
					UEquipTile* equipTile = OwningInventory->GetEquipClass()->GetEquipTileArray()[(int)EEquipType::WEAPON];
					equipTile->SetIconTex(m_IconTex);
					equipTile->SetItemName(m_ItemName);
					equipTile->SetIsEquip(true);
				}

				break;
			}
		}
	}

}
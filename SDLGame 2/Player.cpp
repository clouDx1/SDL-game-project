#include <Windows.h>
#include <iostream>
#include "Player.h"
#include "Texture.h"

#define InventorySize 36
extern PlayerClass* Player;
extern ReadConfig* Config;
extern TextureFont* ExpTip;


PlayerClass::PlayerClass(Texture* Textura, TextureFont* Label, int nX, int nY, int nLevel, int nHp, int nMp, int nAttack, int nDefense)
{
	this->Textura = Textura;
	this->Label = Label;
	this->Position[0] = nX;
	this->Position[1] = nY;
	this->nLevel = nLevel;
	this->nHp = nHp;
	this->nMp = nMp;
	this->nAttack = nAttack;
	this->nDefense = nDefense;
	this->CurrentHp = nHp;
	this->CurrentMp = nMp;
	this->nDirection = 0;
	this->nEvasion = 0;
	this->nStrength = 0;
	this->nHealth = 0;
	this->nAgility = 0;
	this->nOtp = 0;
	this->Experience = 0.0;

	for (int i = 0; i < InventorySize; i++)
	{
		Inventory[i][0] = 0; Inventory[i][1] = 0;
	}
}

bool PlayerClass::InsertItem(int nId, int nAmount)
{
	for (int i = 0; i < 28; i++)
	{
		if (Inventory[i][0] == 0)
		{
			Inventory[i][0] = nId;
			Inventory[i][1] = nAmount;
			return true;
		}
	}
	printf("[Debug] Inventory is full.\n");
	return false;
}
int PlayerClass::GetDamage(int nDamage)
{
		if (this->CurrentHp - nDamage > 0)
		{
			this->CurrentHp -= nDamage;
		}
		//else or die
		return 0;
}
void PlayerClass::UseItem(int nSlotId)
{
	ItemInfo* Item = Config->FindItem(Player->Inventory[nSlotId][0]);
	if (nSlotId < 28) // Inventory Size
	{
		switch (Item->nType)
		{
		case 0: // Sword
		{
			if (!Player->Inventory[30][0])
			{
				Player->Inventory[30][0] = Player->Inventory[nSlotId][0];
				Player->Inventory[30][1] = Player->Inventory[nSlotId][1];
				Player->Inventory[nSlotId][0] = 0;
				Player->Inventory[nSlotId][1] = 0;
				Player->AddItemStats(Item);
			}
			else
				printf("[Debug] Item already equiped.\n");
		}
		break;
		case 1: // Armor
		{
			if (!Player->Inventory[32][0])
			{
				Player->Inventory[32][0] = Player->Inventory[nSlotId][0];
				Player->Inventory[32][1] = Player->Inventory[nSlotId][1];
				Player->Inventory[nSlotId][0] = 0;
				Player->Inventory[nSlotId][1] = 0;
				Player->AddItemStats(Item);
			}
			else
				printf("[Debug] Item already equiped.\n");
		}
		break;
		default:
			printf("[Debug] Error unknown type of item.\n");
		}
	}
	else //Equipment 
	{
		if (Player->InsertItem(Player->Inventory[nSlotId][0], Player->Inventory[nSlotId][1]))
		{
			Player->Inventory[nSlotId][0] = 0;
			Player->RemoveItemStats(Item);
		}
	}
}
void PlayerClass::AddItemStats(ItemInfo* Item)
{
	Player->nAttack += Item->nAttack;
	Player->nDefense += Item->nDef;
	Player->nStrength += Item->nStr;
	Player->nHealth += Item->nHth;
	Player->nEvasion += Item->nEva;
	Player->nAgility += Item->nAgi;
	Player->nOtp += Item->nOtp;
}

void PlayerClass::RemoveItemStats(ItemInfo* Item)
{
	Player->nAttack -= Item->nAttack;
	Player->nDefense -= Item->nDef;
	Player->nStrength -= Item->nStr;
	Player->nHealth -= Item->nHth;
	Player->nEvasion -= Item->nEva;
	Player->nAgility -= Item->nAgi;
	Player->nOtp -= Item->nOtp;
}

void PlayerClass::AllotExp(float fExp)
{
	this->Experience += fExp;
	if (this->Experience >= 100.0)
	{
		this->nLevel++;
		this->Experience -= 100.0;
		char NewLevel[15];
		sprintf(NewLevel, "Lv. %d", this->nLevel);
		Player->Label->EditTextureFont(NewLevel, 10, 0, 255, 255);
	}
	char NewExp[30];
	sprintf(NewExp, "Experience %.2f%% / 100.00%%", this->Experience);
	ExpTip->EditTextureFont(NewExp, 10, 255, 255, 255);
}
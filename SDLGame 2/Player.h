#pragma once
#include "ReadConfig.h"

class PlayerClass
{
public:
	PlayerClass(Texture* Textura, TextureFont* Label, int nX, int nY, int nLevel, int nHp, int nMp, int nAttack, int nDefense);
	Texture* Textura;
	TextureFont* Label;
	char nLevel;
	char nHp;
	char CurrentHp;
	char CurrentMp;
	char nMp;
	int nAttack;
	int nEvasion;
	int nStrength;
	int nHealth;
	int nAgility;
	int nDefense;
	int Position[2];
	int nDirection;
	int nOtp;
	int Skill[5][3];
	float Experience;
	bool bTalkingToNpc;
	TextureFont* NpcDialog;
	Texture* NpcDialogBackground;
	int GetDamage(int nDamage);
	int Inventory[36][2];
	bool InsertItem(int nId, int nAmount);
	bool DeleteItem(int nSlot);
	void UseItem(int nSlot);
	void AddItemStats(ItemInfo* Item);
	void RemoveItemStats(ItemInfo* Item);
	void AllotExp(float fExp);
};

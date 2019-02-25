#pragma once
#include <map>
#include "Texture.h"


class ItemInfo
{
public:
	int nId;
	int nName;
	int nDesc;
	int nType;
	int nTexture;
	int nImage;
	int nAttack;
	int nEva;
	int nDef;
	int nOtp;
	int nStr;
	int nHth;
	int nAgi;
	char szName[20];
	char szDesc[20];
	Texture* Textura;
	Texture* Icon;
};

class ReadConfig
{
public:
	std::map <int, ItemInfo*> ItemInfoMap;
	ReadConfig();
	bool AddItem(int nID, ItemInfo* Item);
	ItemInfo* FindItem(int nId);
	~ReadConfig();
};


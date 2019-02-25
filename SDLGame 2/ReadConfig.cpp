#include "ReadConfig.h"
#include <fstream>
#include <string>

extern SDL_Renderer* gRenderer;

using namespace std;
ReadConfig::ReadConfig()
{
	ifstream Plik, Plik2, Plik3;
	Plik.open("Config//Item.txt");
	Plik2.open("Config//ItemName.txt");
	Plik3.open("Config//ItemDesc.txt");
	if (Plik.is_open() && Plik2.is_open() && Plik3.is_open())
	{
		string Line;
		while (!Plik.eof())
		{
			getline(Plik, Line);
			ItemInfo* Item = new ItemInfo();
			sscanf_s(Line.c_str(), "(Index %d)(Name %d)(Desc %d)(Type %d)(Texture %d)(Image %d)(Attack %d)(Eva %d)(Def %d)(Otp %d)(Str %d)(Hth %d)(Agi %d)", 
				&Item->nId, &Item->nName, &Item->nDesc,&Item->nType, &Item->nTexture, &Item->nImage, &Item->nAttack, &Item->nEva, &Item->nDef, &Item->nOtp, &Item->nStr, &Item->nHth, &Item->nAgi);

			if (Item->nTexture)
			{
				char teporary_texture[30];
				sprintf(teporary_texture, "Data//Image//ItemTexture%d.png", Item->nTexture);
				Item->Textura = new Texture(teporary_texture, gRenderer);
			}
			if (Item->nImage)
			{
				char teporary_icon[30];
				sprintf(teporary_icon, "Data//Image//ItemIcon%d.png", Item->nImage);
				Item->Icon = new Texture(teporary_icon, gRenderer);
			}
			this->AddItem(Item->nId, Item);
		}

		while (!Plik2.eof())
		{
			getline(Plik2, Line);
			int nBegin = 0;
			int nEnd = 0;
			for (int i = 0; i < Line.length(); i++)
			{
				if (Line[i] == '"' && !nBegin)
				{
					nBegin = i + 1;
				}
				else if (Line[i] == '"' && !nEnd)
					nEnd = i-3;
			}

			int nId = atoi(Line.substr(0, nBegin).c_str());
			string Temporary = Line.substr(nBegin, nEnd);
			ItemInfo* Item = ReadConfig::FindItem(nId);
			if (Item)
				strcpy(Item->szName, Temporary.c_str());
		}

		while (!Plik3.eof())
		{
			getline(Plik3, Line);
			int nBegin = 0;
			int nEnd = 0;
			for (int i = 0; i < Line.length(); i++)
			{
				if (Line[i] == '"' && !nBegin)
				{
					nBegin = i + 1;
				}
				else if (Line[i] == '"' && !nEnd)
					nEnd = i - 3;
			}

			int nId = atoi(Line.substr(0, nBegin).c_str());
			string Temporary = Line.substr(nBegin, nEnd);
			ItemInfo* Item = ReadConfig::FindItem(nId);
			if (Item)
				strcpy(Item->szDesc, Temporary.c_str());
		}
	}
	else
		printf("[Config] Couldnt open file.\n");

}

bool ReadConfig::AddItem(int nID,ItemInfo* Item)
{
	printf("(Index %d)(Name %d)(Desc %d)(Type %d)(Texture %d)(Image %d)(Attack %d)(Eva %d)(Def %d)(Otp %d)(Str %d)(Hth %d)(Agi %d);\n", Item->nId, Item->nName, Item->nDesc, Item->nType, Item->nTexture, Item->nImage, Item->nAttack, Item->nEva, Item->nDef, Item->nOtp, Item->nStr, Item->nHth, Item->nAgi);
	this->ItemInfoMap.insert(std::pair<int, ItemInfo*>(nID, Item));
	return true;
}

ItemInfo* ReadConfig::FindItem(int nId)
{
		auto it = ItemInfoMap.find(nId);
		if (it != ItemInfoMap.end())
		{
			return it->second;
		}
		return 0;
}

ReadConfig::~ReadConfig()
{
}

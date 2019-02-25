#include <thread>sword
#include <map>

#include "Defines.h"
#include "CreateMap.h"
#include "ReadConfig.h"
#include "MainGame.h"
#include "Surface.h"
#include "Texture.h"
#include "Fps.h"
#include "Bullet.h"
#include "Monster.h"
#include "Player.h"

#define MapInfo_Collision 1
#define CollisionType_Player 0
#define CollisionType_Monster 1
#define CollisionType_Bullet 2

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define BlockSize 40
#define NpcSize 40
#define WalkDelay 10
#define RecoverDelay 2000
#define HealDelay 500
#define InventorySize 36
#define EmptySlot 666
#define WeatherRain 1
#define WeatherStorm 2

class PlayerClass;
class MonsterClass;
class NpcClass;
class BulletClass;

std::map <int, MonsterClass*> MonsterMap;
std::map <int, NpcClass*> NpcMap;
extern std::map <int, BulletClass*> BulletMap;
//std::map <int, BulletClass*> BulletMap;
PlayerClass* Player;
ReadConfig* Config;
// <------------------------ Jakies zmienne	
int ViewX = 0;
int ViewY = 0;
extern int WindowSize[2];
extern int nMouseCoords[2];
extern SDL_Window* Window;
extern SDL_Surface* Ekran;
extern SDL_Surface* EkranGracza;
extern SDL_Renderer* gRenderer;
extern SDL_Event Event;
extern char GameState;
const char* Map;
int* MapInfo;
int MapWidth;
int MapSize;
bool bLoadedMonsters = false;
bool bLoadedNpcs = false;
bool bStatsWindow = false;
bool bInventoryWindow = false;
bool bControlTipsWindow = true;
unsigned char Weahter = WeatherStorm;

int nRadius = 10;
int nMaxMonsterId = 0;
int nMaxNpcId = 0;
int ClickCount = EmptySlot;
// <------------------------ Timers
DWORD RecoverSleep;
DWORD HealSleep;
DWORD LightningSleep;
DWORD LightningTransparency;
extern DWORD WalkSleep;
extern DWORD NpcTalkSleep;
extern DWORD AttackSleep;
DWORD ClickTimer;
// <------------------------ Texture defines
Texture* PlayerBar;
Texture* MonsterHpView;
Texture* MonsterHpBar;
Texture* HealTexutre;
Texture* BleedingTexture;
Texture* StatsWindow;
Texture* Sword;
Texture* Inventory;
Texture* HpBar;
Texture* MpBar;
Texture* ExpBar;
Texture* ExpBar1;
Texture* Lightning;
Texture* BlockSnow;
Texture* BlockSnowman;
Texture* BlockTree;
Texture* BlockBush;
Texture* BlockCar;
Texture* BlockWall;
Texture* BlockDirt;
Texture* BlockSand;
Texture* Point;
Texture* TestBullet;
Texture* Bullet1;
Texture* FlameSpin;
Texture* BlockSkull;
Texture* ItemTipWindow;
Texture* ControlTips;
Texture* Rain1;
Texture* Rain2;
Texture* Darkness;
TextureFont* ItemTipTextName;	// Inventory
TextureFont* ItemTipTextDesc;
TextureFont* ItemTipTextType;
TextureFont* ItemTipTextAttack;
TextureFont* ItemTipTextEvasion;
TextureFont* ItemTipTextDefense;
TextureFont* ItemTipTextOtp;
TextureFont* ItemTipTextStr;
TextureFont* ItemTipTextHealth;
TextureFont* ItemTipTextAgility;
TextureFont* ExpTip;
TextureFont* PlayerStatsDisplay; // Stats
Texture* FlameTexture;
// <------------------------ Inventory slot coordinates
int InventoryCoordsX[36] = {19,		65,		113,	161,	19,		65,		113,	161,	 19,	65,		113,	161,	 19,	65,		113,	161, 	 19,	65,		113,	161,  	 19,	65,		113,	161,    19,		65,		113,	161/* KONIEC SLOTOW -> Teraz Inv*/,56,	96,	146,	17,		56,		96,		146,	56};
int InventoryCoordsY[36] = {238,	238,	238,	238,	286,	286,	286,	286,	334,	334,	334,	334,	 381,	381,	381,	381,	 430,	430,	430,	430,	 477,	477,	477,	477,	526,	526,	526,	526/* KONIEC SLOTOW -> Teraz Inv*/,64,	64,	64,		104,	104,	104,	108,	144};

enum Skill
{
	timer,
	state,
};


class NpcClass
{
public:
	NpcClass(Texture* Textura, int nX, int nY);
	~NpcClass();
	Texture* Textura;
	Texture* DialogTexture;
	Texture* DialogBackGround;
	TextureFont* DialogText;
	int Position[2];
};

NpcClass::NpcClass(Texture* Textura, int nX, int nY)
{
	this->Textura = Textura;
	this->Position[0] = nX;
	this->Position[1] = nY;
}




bool GetCollision(int nType,int nNewCoordinateX, int nNewCoordinateY, int nSizeX, int nSizeY)
{
	char nX = 0;
	char nY = 0;
	bool Result = false;
	int nCouter = 0;
	for (int i = 0; i <= MapSize; i++)
	{
		if (nCouter == MapWidth)
		{
			nX = 0;
			nCouter = 0;
			nY++;
			continue;
		}
		if (MapInfo[i] & MapInfo_Collision)
		{
			if ((nX * BlockSize) < nNewCoordinateX + nSizeX && (nX * BlockSize) > nNewCoordinateX - BlockSize)
				if ((nY * BlockSize) < nNewCoordinateY + nSizeY && (nY * BlockSize) > nNewCoordinateY - BlockSize)
					return true;
		}
		nX++;
		nCouter++;
	}

	for (auto &i : MonsterMap)
	{
		MonsterClass* pTemp = i.second;
		if (pTemp)
		{
			if ((pTemp->Position[0] < nNewCoordinateX + NpcSize ) && (pTemp->Position[0] > nNewCoordinateX - BlockSize))
				if ((pTemp->Position[1] < nNewCoordinateY + NpcSize) && (pTemp->Position[1] > nNewCoordinateY - BlockSize))
				{
					if (nType == CollisionType_Bullet)
					{
						pTemp->GetDamage(Player->nAttack);
					}
						Result = true;
				}
		}

	}
	return Result;
}



void CreateNpc(int nID, Texture* Textura, int nX, int nY)
{
	NpcClass* Npc = new NpcClass(Textura, nX, nY);
	Npc->DialogBackGround = new Texture("Data//Image//DialogWindow.png", gRenderer);
	Npc->DialogText = new TextureFont("Data//Font//Gameplay.ttf", gRenderer, "Hello iam npc...", 10, 255, 255, 255);

	Npc->DialogTexture = new Texture("Data//Image//DialogTexture.png", gRenderer);
	NpcMap.insert(std::pair<int, NpcClass*>(nID, Npc));
}


void DrawNpc()
{
	for (auto &i : NpcMap)
	{
		NpcClass* pTemp = i.second;
		if (pTemp)
		{
			pTemp->Textura->ShowTexture(pTemp->Position[0] - ViewX, pTemp->Position[1] - ViewY, BlockSize, BlockSize);
			pTemp->DialogTexture->ShowTexture(pTemp->Position[0] - ViewX - 5, pTemp->Position[1] - ViewY - 25,35,30);
		}

	}
}

void CreateMonster(int nID,Texture* Textura, TextureFont* Label,int nX, int nY, char nLevel, char nHp, char nMp, char nAttack, char nDefense,int nChaseRange, float fExperience)
{
	MonsterClass* Monster = new MonsterClass(nID,Textura,Label,nX,nY,nLevel,nHp,nMp,nAttack,nDefense, nChaseRange, fExperience);
	Monster->SetWalk(20, 20, 4000);
	MonsterMap.insert(std::pair<int, MonsterClass*>(nID, Monster));
}

void TalkToNpc()
{
	
	for (auto &i : NpcMap)
	{
		NpcClass* pTemp = i.second;
		if (pTemp)
		{
			if ((pTemp->Position[0] < Player->Position[0] + NpcSize + 1) && (pTemp->Position[0] > Player->Position[0] - BlockSize - 1))
				if ((pTemp->Position[1] < Player->Position[1] + NpcSize + 1) && (pTemp->Position[1] > Player->Position[1] - BlockSize - 1))
				{
					//if(Player->NpcDialog)
						//Player->NpcDialog->~TextureFont();

					//if (Player->NpcDialogBackground)
						//Player->NpcDialogBackground->~Texture();
					
					Player->NpcDialogBackground = pTemp->DialogBackGround;
					Player->NpcDialog = pTemp->DialogText;

					Player->bTalkingToNpc = true;
					NpcTalkSleep = GetTickCount() + 1000;
			}
		}

	}
}
void AttackMonster(int nX, int nY)
{
	for (auto &i : MonsterMap)
	{
		MonsterClass* pTemp = i.second;
		if (pTemp)
		{
			if ((pTemp->Position[0]  < Player->Position[0] + NpcSize + 1) && (pTemp->Position[0]  > Player->Position[0] - BlockSize - 1))
				if ((pTemp->Position[1] < Player->Position[1] + NpcSize + 1) && (pTemp->Position[1] > Player->Position[1] - BlockSize - 1))
				{
						Sword->SetAnimation(40, 0, 40, 0, 1, 50);

						pTemp->Textura->SetAnimation(40, 0, 40, 0, 2, 50);

						BleedingTexture->SetAnimation(0, 0, 50, 0, 2, 200);
						BleedingTexture->Tick = GetTickCount() + 400;

						AttackSleep = GetTickCount() + 100;
						pTemp->WalkSleep = GetTickCount() + 5000;
						printf("[DEBUG] Attack %d-%d=%d\n", pTemp->CurrentHp, Player->nAttack, pTemp->CurrentHp - Player->nAttack);
						pTemp->GetDamage(Player->nAttack);
						return;
				}
		}

	}
	
	int nFixedMouseCoords[2] = { nMouseCoords[0] + ViewX, nMouseCoords[1] + ViewY };
	BulletClass* Bullet = new BulletClass(nFixedMouseCoords, Player->Position, 30, 15, 0);
}


void DrawBullets()
{
	for (auto &i : BulletMap)
	{
		BulletClass* pTemp = i.second;
		if (pTemp)
		{
			pTemp->Move();
			SDL_SetTextureAlphaMod(TestBullet->Name, 255);
			pTemp->Teksutra->fAngle = pTemp->fAngle * 180 / 3.14;
			pTemp->Teksutra->ShowAnimatedTexture(pTemp->Position[0] - ViewX, pTemp->Position[1] - ViewY, 20, 20, 60, 0);
		}

	}
}


void DrawMonsters()
{
	for (auto &i : MonsterMap)
	{
		MonsterClass* pTemp = i.second;
		if (pTemp)
		{
			pTemp->Walk();
			pTemp->Textura->ShowAnimatedTexture(pTemp->Position[0] - ViewX, pTemp->Position[1] - ViewY, BlockSize, BlockSize,0,0);
			pTemp->Label->ShowTexture(pTemp->Position[0] - ViewX - pTemp->Label->nTextSize[0]/3, pTemp->Position[1] - ViewY - 15);
			MonsterHpView->ShowTexture(pTemp->Position[0] - ViewX - 20, pTemp->Position[1] - ViewY - 32, 83, 15);
			int addX = 0;
			if (pTemp->AttackTimer > GetTickCount())//trash way
			{
				int nX = 0;
				if (FlameTexture->fAngle > -90 && FlameTexture->fAngle < 90)
				{
					nX = pTemp->Position[0] - ViewX + 40;
				}
				else
				{
					nX = pTemp->Position[0] - ViewX - 40;
				}
				FlameTexture->ShowAnimatedTexture(nX, pTemp->Position[1] - ViewY, BlockSize, BlockSize, 80, 0);
			}
			for (int i = 0; i <  pTemp->CurrentHp * 10 / pTemp->nHp  ; i++)
			{
				MonsterHpBar->ShowTexture(pTemp->Position[0] - ViewX - 18 + addX, pTemp->Position[1] - ViewY - 30, 7, 11);
				addX += 8;
			}
		}
		
	}
}

DWORD WINAPI GetKeyThread(LPVOID lpParam)
{
	while (GameState == 1)
	{
		if (SDL_GetWindowFlags(Window) & SDL_WINDOW_INPUT_FOCUS)
		{
			if (GetTickCount() >= HealSleep)
			{
				if (GetAsyncKeyState(0x31)) // <--- '1 Key'
				{
					if (Player->CurrentMp >= 10 && Player->CurrentHp < Player->nHp)
					{
						if (Player->CurrentHp + 5 <= Player->nHp)
							Player->CurrentHp += 5;
						else
							Player->CurrentHp += Player->nHp - Player->CurrentHp;
						
						Player->CurrentMp -= 10;
						HealTexutre->SetAnimation(0, 0, 40, 0, 2, 200);
						HealTexutre->Tick = GetTickCount() + 400;
						HealSleep = GetTickCount() + HealDelay;
					}
				}
				if (GetAsyncKeyState(0x32)) // <--- '2 Key'
				{
					HealSleep = GetTickCount() + 100;
					Player->AllotExp(1.0);
				}
				if (GetAsyncKeyState(0x33)) // <--- '3 Key'
				{
					if (Player->Skill[0][state] == 0)
					{
						Player->Skill[0][timer] = 500;
						Player->Skill[0][state] = 1;
					}
				}
			}
			if (GetTickCount() >= WalkSleep)
			{
				if (GetAsyncKeyState(0x57)) // <--- 'W' Key
				{

					Player->Textura->SetAnimation(40, 0, 40, 0, 2, 100);
					if (Player->bTalkingToNpc && GetTickCount() > NpcTalkSleep)
					{
						NpcTalkSleep = GetTickCount() + 1000;
						Player->bTalkingToNpc = false;
					}
					if (!GetCollision(CollisionType_Player,Player->Position[0], Player->Position[1] - 1,NpcSize, NpcSize))
					{
						Player->Position[1] -= 1;
						WalkSleep = GetTickCount() + WalkDelay;
					}
				}
				if (GetAsyncKeyState(0x53)) // <--- 'S' Key
				{
					Player->Textura->SetAnimation(40, 0, 40, 0, 2, 100);
					if (Player->bTalkingToNpc && GetTickCount() > NpcTalkSleep)
					{
						NpcTalkSleep = GetTickCount() + 1000;
						Player->bTalkingToNpc = false;
					}
					if (!GetCollision(CollisionType_Player, Player->Position[0], Player->Position[1] + 1,NpcSize, NpcSize))
					{
						Player->Position[1] += 1;
						WalkSleep = GetTickCount() + WalkDelay;
					}
				}
				if (GetAsyncKeyState(0x41)) // <--- 'A' Key
				{
					Player->Textura->SetAnimation(40, 0, 40, 0, 2, 100);
					Player->nDirection = 1;

					if (Player->bTalkingToNpc && GetTickCount() > NpcTalkSleep)
					{
						NpcTalkSleep = GetTickCount() + 1000;
						Player->bTalkingToNpc = false;
					}
					if (!GetCollision(CollisionType_Player, Player->Position[0] - 1, Player->Position[1], NpcSize, NpcSize))
					{
						Player->Position[0] -= 1;
						WalkSleep = GetTickCount() + WalkDelay;
					}
				}
				if (GetAsyncKeyState(0x44)) // <--- 'D' Key
				{
					Player->Textura->SetAnimation(40, 0, 40, 0, 2, 100);
					Player->nDirection = 0;

					if (Player->bTalkingToNpc && GetTickCount() > NpcTalkSleep)
					{
						NpcTalkSleep = GetTickCount() + 1000;
						Player->bTalkingToNpc = false;
					}
					if (!GetCollision(CollisionType_Player, Player->Position[0] + 1, Player->Position[1], NpcSize, NpcSize))
					{
						Player->Position[0] += 1;
						WalkSleep = GetTickCount() + WalkDelay;
					}
				}

				if (GetAsyncKeyState(0x20)) // <--- 'SpaceKey'
				{
					if (GetTickCount() > NpcTalkSleep)
					{
						if (!Player->bTalkingToNpc)
							TalkToNpc();
						else
						{
							Player->bTalkingToNpc = false;
							NpcTalkSleep = GetTickCount() + 1000;
						}

					}
					WalkSleep = GetTickCount() + WalkDelay;
				}

				if (GetAsyncKeyState(0x49)) // <--- 'I' Key
				{
					if (bInventoryWindow)
						bInventoryWindow = false;
					else
						bInventoryWindow = true;
					WalkSleep = GetTickCount() + 500;
				}
				if (GetAsyncKeyState(0x43)) // <--- 'C' Key
				{
					if (bStatsWindow)
						bStatsWindow = false;
					else
						bStatsWindow = true;
					WalkSleep = GetTickCount() + 500;
				}

				if (GetAsyncKeyState(0x1B)) // <--- 'Esc' Key
				{
					GameState = 0;
				}
			}
		}
		Sleep(10);
	}
	return 0;
}

//<--------------------- Rain
float fRainPos[500][3] = { 0 };

void Rain(int nFallSpeed, int nFrequency) // <- max frequency 500
{
	for (int i = 0; i < nFrequency; i++)
	{
		if (fRainPos[i][0] == 0 && GetTickCount() > fRainPos[i][2])
		{
			if (rand() % 50 == 1)
			{
				fRainPos[i][0] = rand() % WindowSize[0] + 1 + ViewX;
				fRainPos[i][1] = rand() % 300 + ViewY;
				fRainPos[i][2] = 0;
			}	
		}
		else
		{
			if (fRainPos[i][1] + nFallSpeed <= WindowSize[1] + ViewY && fRainPos[i][2] == 0)
			{
				if (rand() % 80 == 0)
				{
					fRainPos[i][2] = GetTickCount() + 500;
				}
				else
				{
						fRainPos[i][1] += nFallSpeed;
						Rain1->ShowTexture(fRainPos[i][0] - ViewX, fRainPos[i][1] - ViewY, 3, 6);
				}	
			}
			else
			{
				if (GetTickCount() < fRainPos[i][2])
				{
					Rain2->ShowTexture(fRainPos[i][0] - ViewX, fRainPos[i][1] - ViewY, 10, 20);
				}
				else
				fRainPos[i][0] = 0;	
			}	
		}
	}
}
//<--------------------- Load textures
void LoadTextures()
{
	Player = new PlayerClass(new Texture("Data//Image//Player.png", gRenderer), new TextureFont("Data//Font//Gameplay.ttf", gRenderer, "Lv. 1", 10, 0, 255, 255), 50, 50, 1, 100, 100, 10, 10);
	HealTexutre = new Texture("Data//Image//Healed.png", gRenderer);
	BleedingTexture = new Texture("Data//Image//Bleeding.png", gRenderer);
	PlayerBar = new Texture("Data//Image//PlayerBar.bmp", gRenderer);
	HpBar = new Texture("Data//Image//HpBar.bmp", gRenderer);
	MpBar = new Texture("Data//Image//MpBar.bmp", gRenderer);
	ExpBar = new Texture("Data//Image//ExpBar.png", gRenderer);
	ExpBar1 = new Texture("Data//Image//ExpBar1.bmp", gRenderer);
	MonsterHpView = new Texture("Data//Image//MonsterHp.png", gRenderer);
	MonsterHpBar = new Texture("Data//Image//MonsterHpBar.png", gRenderer);
	BlockSnow = new Texture("Data//Image//Snow.png", gRenderer);
	BlockSnowman = new Texture("Data//Image//Snowman.png", gRenderer);
	StatsWindow = new Texture("Data//Image//StatsWindow.png", gRenderer);
	BlockTree = new Texture("Data//Image//Tree.png", gRenderer);
	BlockBush = new Texture("Data//Image//Bush.png", gRenderer);
	BlockCar = new Texture("Data//Image//Car.bmp", gRenderer);
	BlockWall = new Texture("Data//Image//Block1.png", gRenderer);
	BlockDirt = new Texture("Data//Image//Block2.png", gRenderer);
	BlockSand = new Texture("Data//Image//Block3.png", gRenderer);
	BlockSkull = new Texture("Data//Image//Block4.bmp", gRenderer);
	Inventory = new Texture("Data//Image//Inventory.png", gRenderer);
	Darkness = new Texture("Data//Image//Darkness.png", gRenderer);
	Lightning = new Texture("Data//Image//Lightning.png", gRenderer);
	Sword = new Texture("Data//Image//ItemTexture1.png", gRenderer);
	FlameSpin = new Texture("Data//Image//FlameSpin.png", gRenderer);
	Point = new Texture("Data//Image//Point.png", gRenderer);
	TestBullet = new Texture("Data//Image//TestBullet.png", gRenderer);
	Bullet1 = new Texture("Data//Image//Bullet1.png", gRenderer);
	Rain1 = new Texture("Data//Image//Rain1.png", gRenderer);
	Rain2 = new Texture("Data//Image//Rain2.png", gRenderer);
	ControlTips = new Texture("Data//Image//ControlTips.bmp", gRenderer);
	ItemTipWindow = new Texture("Data//Image//InventoryTip.bmp", gRenderer);
	ItemTipTextName = new TextureFont("Data//Font//Roboto-Bold.ttf", gRenderer, "Name", 14, 255, 255, 255);
	ItemTipTextDesc = new TextureFont("Data//Font//Roboto-Bold.ttf", gRenderer, "Desc", 12, 255, 255, 255);
	ItemTipTextType = new TextureFont("Data//Font//Roboto-Bold.ttf", gRenderer, "Type", 12, 255, 255, 255);
	ItemTipTextAttack = new TextureFont("Data//Font//Roboto-Bold.ttf", gRenderer, "Attack", 12, 255, 255, 255);
	ItemTipTextEvasion = new TextureFont("Data//Font//Roboto-Bold.ttf", gRenderer, "Evasion", 12, 255, 255, 255);
	ItemTipTextDefense = new TextureFont("Data//Font//Roboto-Bold.ttf", gRenderer, "Defense", 12, 255, 255, 255);
	ItemTipTextOtp = new TextureFont("Data//Font//Roboto-Bold.ttf", gRenderer, "Otp", 12, 255, 255, 255);
	ItemTipTextStr = new TextureFont("Data//Font//Roboto-Bold.ttf", gRenderer, "Str", 12, 255, 255, 255);
	ItemTipTextHealth = new TextureFont("Data//Font//Roboto-Bold.ttf", gRenderer, "Health", 12, 255, 255, 255);
	ItemTipTextAgility = new TextureFont("Data//Font//Roboto-Bold.ttf", gRenderer, "Agility", 12, 255, 255, 255);
	PlayerStatsDisplay = new TextureFont("Data//Font//Roboto-Bold.ttf", gRenderer, "Attack", 12, 255, 255, 255);
	ExpTip = new TextureFont("Data//Font//Roboto-Bold.ttf", gRenderer, "Experience 0% / 100.00%", 12, 255, 255, 255);
	FlameTexture = new Texture("Data//Image//FlameMob.png", gRenderer);
}

//<--------------------- Main game function
void CreateGame()
{
	LoadTextures();
	Config = new ReadConfig();
	LoadMap();
	MapInfo = new int[strlen(Map)];
	MapSize = strlen(Map);
	CreateThread(NULL,0, GetKeyThread,0 ,0 ,0);
	Fps* LimitFps = new Fps(60);
	ViewY = 0;
	ViewX = 0;

	Player->InsertItem(1, 1);
	Player->InsertItem(2, 1);

	while (GameState == 1)
	{
		LimitFps->FrameBegin();
		SDL_RenderClear(gRenderer);
		SDL_GetMouseState(&nMouseCoords[0], &nMouseCoords[1]);
		
		while (SDL_PollEvent(&Event))
		{
			if (Event.type == SDL_QUIT)
				GameState = 3;
			if (Event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (Event.button.button == SDL_BUTTON_RIGHT)
				{
					int nFixedMouseCoords[2] = { nMouseCoords[0] + ViewX, nMouseCoords[1] + ViewY };
					float Additional = -0.40;
					for (int i = 0; i < 8; i++)
					{
						BulletClass* Bullet = new BulletClass(nFixedMouseCoords, Player->Position, 30, 15, Additional);
						Additional += 0.10;
					}
				}
				if (Event.button.button == SDL_BUTTON_LEFT)
				{
					bool DialogClicked = false;
					if (bControlTipsWindow) bControlTipsWindow = false;

					if(bInventoryWindow)
					for (int i = 0; i < InventorySize; i++)
					{
						if (Player->Inventory[i][0] != 0)
						{
							if (nMouseCoords[0] <= 800 + InventoryCoordsX[i] + 35 && nMouseCoords[0] >= 800 + InventoryCoordsX[i])
								if (nMouseCoords[1] <= InventoryCoordsY[i] + 35 && nMouseCoords[1] >= InventoryCoordsY[i])
								{
									if (ClickCount == EmptySlot)
									{
										ClickCount = i;
										ClickTimer = GetTickCount() + 1000;
									}
									else if (ClickCount == i)
									{
										Player->UseItem(i);
									}
									else
										ClickCount = EmptySlot;
									
									DialogClicked = true;
								}

						}
					}

					if (GetTickCount() > AttackSleep && !DialogClicked)
					{
						AttackMonster(nMouseCoords[0], nMouseCoords[1]); // also shoot
					}
				}
			}
		}
	
		if(GetTickCount() > ClickTimer)
		{
			ClickCount = EmptySlot;
		}


		char nY = 0;
		char nX = 0;
		for (int i = 0; i <= MapSize; i++)
		{

				switch (Map[i])
				{
				case '$':
				{
					if (Player->Position[0] > WindowSize[0] / 2)
					{
						//i += 1 + (PlayerPos[0] - (WindowSize[0] / 2)) / 20;
						if (nX * BlockSize >= Player->Position[0] + (WindowSize[0] / 2))
							ViewX = Player->Position[0] - (WindowSize[0] / 2);
						if (ViewX < 0) ViewX = 0;
					}
					nX = 0;
					nY++;
					if (Player->Position[1] > WindowSize[1] / 2)
					{
						if (nY * BlockSize>= Player->Position[1] + (WindowSize[1] / 2))
						{
							ViewY = Player->Position[1] - (WindowSize[1] / 2);
							if (ViewY < 0) ViewY = 0;
						}
					}
					
					if (!MapWidth)
						MapWidth = i;
						
				}
				break;
				case ' ':
				{
					BlockDirt->ShowTexture((nX * BlockSize) - ViewX, (nY * BlockSize) - ViewY, BlockSize, BlockSize);

					nX++;
				}
				break;
				case '-':
				{
					BlockSnow->ShowTexture((nX * BlockSize) - ViewX, (nY * BlockSize) - ViewY, BlockSize, BlockSize);
					nX++;
				}
				break;
				case 'N':
				{
					MapInfo[i] |= MapInfo_Collision;
					BlockDirt->ShowTexture((nX * BlockSize) - ViewX, (nY * BlockSize) - ViewY, BlockSize, BlockSize);
					if (!bLoadedNpcs)
					{
						CreateNpc(nMaxNpcId, new Texture("Data//Image//Npc1.png", gRenderer), (nX * BlockSize) - ViewX, (nY * BlockSize) - ViewY);
						nMaxNpcId++;
					}
					nX++;
				}
				break;
				case '1':
				{
					BlockDirt->ShowTexture((nX * BlockSize) - ViewX, (nY * BlockSize) - ViewY, BlockSize, BlockSize);
					if (!bLoadedMonsters)
					{																																																//char nLevel, char nHp, char nMp, char nAttack, char nDefense, int nChaseRange,float fExperience)
						CreateMonster(nMaxMonsterId, new Texture("Data//Image//Monster1.png", gRenderer), new TextureFont("Data//Font//Gameplay.ttf", gRenderer, "[Lv. 1] Vulgar", 10, 255, 255, 255), (nX * BlockSize) - ViewX, (nY * BlockSize),1, 100, 100, 5, 1,150, 75.0);
						nMaxMonsterId++;
					}
					nX++;
				}
				break;
				case '*':
				{
					BlockSand->ShowTexture((nX * BlockSize) - ViewX, (nY * BlockSize) - ViewY, BlockSize, BlockSize);
					nX++;
				}
				break;
				case '@':
				{
					MapInfo[i] |= MapInfo_Collision;
					BlockSkull->ShowTexture((nX * BlockSize) - ViewX, (nY * BlockSize) - ViewY, BlockSize, BlockSize);
					nX++;
				}
				break;
				case 'T':
				{
					MapInfo[i] |= MapInfo_Collision;
					BlockDirt->ShowTexture((nX * BlockSize) - ViewX, (nY * BlockSize) - ViewY, BlockSize, BlockSize);
					BlockTree->ShowTexture((nX * BlockSize) - ViewX, (nY * BlockSize) - ViewY, BlockSize, BlockSize);
					nX++;
				}

				break;
				case 'S':
				{
					MapInfo[i] |= MapInfo_Collision;
					BlockSnow->ShowTexture((nX * BlockSize) - ViewX, (nY * BlockSize) - ViewY, BlockSize, BlockSize);
					BlockSnowman->ShowTexture((nX * BlockSize) - ViewX, (nY * BlockSize) - ViewY, BlockSize, BlockSize);
					nX++;
				}
				break;
				case 'B':
				{
					MapInfo[i] |= MapInfo_Collision;
					BlockDirt->ShowTexture((nX * BlockSize) - ViewX, (nY * BlockSize) - ViewY, BlockSize, BlockSize);
					BlockBush->ShowTexture((nX * BlockSize) - ViewX, (nY * BlockSize) - ViewY, BlockSize, BlockSize);
					nX++;
				}
				break;
				case 'C':
				{
					MapInfo[i] |= MapInfo_Collision;
					BlockCar->ShowTexture((nX * BlockSize) - ViewX, (nY * BlockSize) - ViewY, BlockSize, BlockSize);
					nX++;
				}
				break;
				case '#':
				{
					MapInfo[i] |= MapInfo_Collision;
					BlockWall->ShowTexture((nX * BlockSize) - ViewX, (nY * BlockSize) - ViewY, BlockSize, BlockSize);
					nX++;
				}
				break;
				}
		}
		
		if (!bLoadedMonsters)
		{
			bLoadedMonsters = true;
			printf("[DEBUG] Monsters loaded.\n");
		}

		if (!bLoadedNpcs)
		{
			bLoadedNpcs = true;
			printf("[DEBUG] NPC's loaded.\n");
		}

		DrawMonsters();
		DrawNpc();
		
		//<------------------------------------- Weather -------------------------------------------
		if (Weahter == WeatherRain) // <- pewnie useless
			Rain(5, 400);
		else if (Weahter == WeatherStorm)
		{
			//<---------------------- Lightning	
			if (rand() % 150 == 1 && LightningSleep == 0)
			{
				LightningSleep = GetTickCount() + 500;
				LightningTransparency = 255;
			}
			if (GetTickCount() < LightningSleep && LightningSleep != 0)
			{
				//if(LightningSleep % 2 == 0)
				SDL_SetTextureAlphaMod(Lightning->Name, LightningTransparency);
				LightningTransparency -= 50;
				Lightning->ShowTexture(0, 0, 1000, 600);
			}
			else
				LightningSleep = 0;


			// <---------------------- Darkne$$$
			int Radius = 50;
			for (int ni = 0; ni < WindowSize[0]; ni += 5)
			{
				for (int ny = 0; ny < WindowSize[1]; ny += 5)
				{
					float distance_to_centre = sqrt((ni - Radius - Player->Position[0] + ViewX + 25)*(ni - Radius - Player->Position[0] + ViewX + 25) + (ny - Radius - Player->Position[1] + ViewY + 25)*(ny - Radius - Player->Position[1] + ViewY + 25));
					if (distance_to_centre < Radius + 100)
					{
						SDL_SetTextureAlphaMod(Darkness->Name, distance_to_centre);
						Darkness->ShowTexture(ni, ny, 5, 5);
					}
					else
					{
						SDL_SetTextureAlphaMod(Darkness->Name, 150);
						Darkness->ShowTexture(ni, ny, 5, 5);
					}
				}
			}
			Rain(9, 500);
		}

		// circle around
		if (Player->Skill[0][state])
		{
			if (Player->Skill[0][state] <= 25)
			{
				if (Player->Skill[0][state] == 1)
				{
					FlameSpin->SetAnimation(0, 0, 15, 0, 4, 200);
					Player->Textura->SetAnimation(120, 0, 40, 0, 3, 100);
				}
					

				Player->Skill[0][state]+=2;
				int Radius = 100;
				for (int ni = 0; ni < WindowSize[0]; ni += 15)
				{
					for (int ny = 0; ny < WindowSize[1]; ny += 15)
					{
						float distance_to_centre = sqrt((ni - Radius - Player->Position[0] + ViewX + 75)*(ni - Radius - Player->Position[0] + ViewX + 75) + (ny - Radius - Player->Position[1] + ViewY + 75)*(ny - Radius - Player->Position[1] + ViewY + 75));
						if ( distance_to_centre <= 4 + Player->Skill[0][state] * 2 && distance_to_centre >= 4 - Player->Skill[0][state] * 2 )
						{
								FlameSpin->ShowAnimatedTexture(ni, ny, 17 + Player->Skill[0][state], 20 + Player->Skill[0][state], 0, 0);
						}
						else if (distance_to_centre <= 5 + Player->Skill[0][state] * 3 && distance_to_centre >= 5 - Player->Skill[0][state] * 3)
						{
							if(rand()%2 == 0)
							FlameSpin->ShowAnimatedTexture(ni, ny, 17 + Player->Skill[0][state], 20 + Player->Skill[0][state], 45, 0);
						}
					}
				}
			}
			else
				Player->Skill[0][state] = 0;
		}
		
		DrawBullets(); //<-avoid darkness
//<------------------------------------- Hp/Mp Bar -----------------------------------------
		PlayerBar->ShowTexture(10 , 10, 143,44);
		int addX = 0;
		for (int i = 0; i < Player->CurrentHp * 10 / Player->nHp; i++)
		{
			HpBar->ShowTexture(55 + addX, 20, 10, 16);
			
			addX += 9;
		}
		addX = 0;
		for (int i = 0; i < Player->CurrentMp * 10 / Player->nMp; i++)
		{
			MpBar->ShowTexture(55 + addX, 40, 10, 6);
			addX += 9;
		}

		if (GetTickCount() > RecoverSleep) // <---------------- Odnawianie sie zycka i many 
		{
			if (Player->CurrentHp + 2 <= Player->nHp)
				Player->CurrentHp += 2;
			
			if (Player->CurrentMp + 2 <= Player->nMp)
				Player->CurrentMp += 2;
			RecoverSleep = GetTickCount() + RecoverDelay;
		}

//<------------------------------------- Exp Bar -----------------------------------------
		
		for (int i = 0; i < (int)Player->Experience; i++)
		{
			ExpBar1->ShowTexture(WindowSize[0] / 2 - 196 + (i * 4), 573, 4, 19);
		}
		ExpBar->ShowTexture(WindowSize[0] / 2 - 200, 570, 400, 25);
		ExpTip->ShowTexture(WindowSize[0] / 2 - 200, 550);
//<------------------------------------- Player display -----------------------------------------		
		if (!Player->nDirection)
		{
			Player->Textura->flipType = SDL_FLIP_NONE;
			Player->Textura->ShowAnimatedTexture(Player->Position[0] - ViewX, Player->Position[1] - ViewY, 40, 40, 0, 0);
			if (Player->Inventory[30][0])
			{
				Sword->flipType = SDL_FLIP_NONE;
				Sword->ShowAnimatedTexture(Player->Position[0] - ViewX + 20, Player->Position[1] - ViewY, 40, 40, 0, 0);
			}
		}
		else
		{
			Player->Textura->flipType = SDL_FLIP_HORIZONTAL;
			Player->Textura->ShowAnimatedTexture(Player->Position[0] - ViewX, Player->Position[1] - ViewY, 40, 40, 0, 0);
			if (Player->Inventory[30][0])
			{
				Sword->flipType = SDL_FLIP_HORIZONTAL;
				Sword->ShowAnimatedTexture(Player->Position[0] - ViewX - 20, Player->Position[1] - ViewY, 40, 40, 0, 0);
			}
		}

		Player->Label->ShowTexture(Player->Position[0] - 10 - ViewX, Player->Position[1] - ViewY - 20);
		
		if(HealTexutre->Tick > GetTickCount())
			HealTexutre->ShowAnimatedTexture(Player->Position[0] - ViewX, Player->Position[1] - ViewY, 40, 40, 0, 0);

		if (BleedingTexture->Tick > GetTickCount())
			BleedingTexture->ShowAnimatedTexture(Player->Position[0] - ViewX - 10, Player->Position[1] - ViewY - 10, 50, 50, 0, 0 );

				
//<------------------------------------- Stats -----------------------------------------
		if (bStatsWindow)
		{
			int nXOffset = WindowSize[0] / 2 - 125 + 20;
			int nYOffset = WindowSize[1] / 2 - 100 + 20;
			StatsWindow->ShowTexture(WindowSize[0] / 2 - 125, WindowSize[1] / 2 - 100, 250, 200);

				char MaxHp[15];
				sprintf(MaxHp, "Health %d / %d", Player->CurrentHp, Player->nHp);
				PlayerStatsDisplay->EditTextureFont(MaxHp, 10, 255, 0, 0);
				PlayerStatsDisplay->ShowTexture(nXOffset, nYOffset);

				char MaxMp[15];
				sprintf(MaxMp, "Mana %d / %d", Player->CurrentMp, Player->nMp);
				PlayerStatsDisplay->EditTextureFont(MaxMp, 10, 0, 0, 255);
				PlayerStatsDisplay->ShowTexture(nXOffset, nYOffset + 15);

				char Attack[15];
				sprintf(Attack, "Attack %d", Player->nAttack);
				PlayerStatsDisplay->EditTextureFont(Attack, 10, 255, 255, 255);
				PlayerStatsDisplay->ShowTexture(nXOffset, nYOffset + 30);

				char Defense[15];
				sprintf(Defense, "Defense %d", Player->nDefense);
				PlayerStatsDisplay->EditTextureFont(Defense, 10, 255, 255, 255);
				PlayerStatsDisplay->ShowTexture(nXOffset, nYOffset + 45);

				char Evasion[15];
				sprintf(Evasion, "Evasion %d", Player->nEvasion);
				PlayerStatsDisplay->EditTextureFont(Evasion, 10, 255, 255, 255);
				PlayerStatsDisplay->ShowTexture(nXOffset, nYOffset + 60);

				char Otp[15];
				sprintf(Otp, "Target Point %d", Player->nOtp);
				PlayerStatsDisplay->EditTextureFont(Otp, 10, 255, 255, 255);
				PlayerStatsDisplay->ShowTexture(nXOffset, nYOffset + 75);

				char Strength[15];
				sprintf(Strength, "Strength %d", Player->nStrength);
				PlayerStatsDisplay->EditTextureFont(Strength, 10, 102, 153, 255);
				PlayerStatsDisplay->ShowTexture(nXOffset + 130, nYOffset + 30);

				char Health[15];
				sprintf(Health, "Health %d", Player->nHealth);
				PlayerStatsDisplay->EditTextureFont(Health, 10, 102, 153, 255);
				PlayerStatsDisplay->ShowTexture(nXOffset + 130, nYOffset + 45);

				char Agility[15];
				sprintf(Agility, "Agility %d", Player->nAgility);
				PlayerStatsDisplay->EditTextureFont(Agility, 10, 102, 153, 255);
				PlayerStatsDisplay->ShowTexture(nXOffset + 130, nYOffset + 60);
			
		}
			
//<------------------------------------- Inventory -----------------------------------------
		if (bInventoryWindow)
		{
			Inventory->ShowTexture(800, 0, 200, 600);

			for (int i = 0; i < InventorySize; i++)
			{
				if (Player->Inventory[i][0] != 0)
				{
					ItemInfo* Item = Config->FindItem(Player->Inventory[i][0]);
					if (Item)
					{
						Item->Icon->ShowTexture(800 + InventoryCoordsX[i], InventoryCoordsY[i], 35, 35);
					}
				}
			}

			//rewrite?
			for (int i = 0; i < InventorySize; i++)
			{
				if (Player->Inventory[i][0] != 0)
				{
					if (nMouseCoords[0] <= 800 + InventoryCoordsX[i] + 35 && nMouseCoords[0] >= 800 + InventoryCoordsX[i])
						if (nMouseCoords[1] <= InventoryCoordsY[i] + 35 && nMouseCoords[1] >= InventoryCoordsY[i])
						{
							ItemInfo* Item = Config->FindItem(Player->Inventory[i][0]);
							ItemTipWindow->ShowTexture(nMouseCoords[0] - 200, nMouseCoords[1], 200, 170);

							int nYoffset = 10;

							ItemTipTextName->EditTextureFont(Item->szName, 16, 255, 0, 0);
							ItemTipTextName->ShowTexture(nMouseCoords[0] - 180, nMouseCoords[1] + nYoffset);
							nYoffset += 25;

							ItemTipTextDesc->EditTextureFont(Item->szDesc, 14, 153, 153, 102);
							ItemTipTextDesc->ShowTexture(nMouseCoords[0] - 180, nMouseCoords[1] + nYoffset);
							nYoffset += 15;

							switch (Item->nType)
							{
								case 0:
								ItemTipTextDesc->EditTextureFont("Class: Sword", 10, 102, 153, 255);
								ItemTipTextDesc->ShowTexture(nMouseCoords[0] - 180, nMouseCoords[1] + nYoffset);
								break;
								case 1:
									ItemTipTextDesc->EditTextureFont("Class: Armor", 10, 102, 153, 255);
									ItemTipTextDesc->ShowTexture(nMouseCoords[0] - 180, nMouseCoords[1] + nYoffset);
									break;
								default:
									ItemTipTextDesc->EditTextureFont("Class: UNKNOWN", 10, 102, 153, 255);
									ItemTipTextDesc->ShowTexture(nMouseCoords[0] - 180, nMouseCoords[1] + nYoffset);
		
							}
							
							if (Item->nAttack) // <------ nie wyswietlanie statow ktorych dany item nie ma
							{
								nYoffset += 15;
								char Attack[10];
								sprintf(Attack, "Attack: %d", Item->nAttack);
								ItemTipTextAttack->EditTextureFont(Attack, 10, 255, 255, 255);
								ItemTipTextAttack->ShowTexture(nMouseCoords[0] - 180, nMouseCoords[1] + nYoffset);
							}
							
							if (Item->nEva)
							{
								nYoffset += 15;
								char Evasion[10];
								sprintf(Evasion, "Evasion: %d", Item->nEva);
								ItemTipTextAttack->EditTextureFont(Evasion, 10, 255, 255, 255);
								ItemTipTextAttack->ShowTexture(nMouseCoords[0] - 180, nMouseCoords[1] + nYoffset);
							}
							
							if (Item->nDef)
							{
								nYoffset += 15;
								char Defense[10];
								sprintf(Defense, "Defense: %d", Item->nDef);
								ItemTipTextDefense->EditTextureFont(Defense, 10, 255, 255, 255);
								ItemTipTextDefense->ShowTexture(nMouseCoords[0] - 180, nMouseCoords[1] + nYoffset);
							}
							
							if (Item->nOtp)
							{
								nYoffset += 15;
								char Otp[15];
								sprintf(Otp, "On Target Point: %d", Item->nOtp);
								ItemTipTextOtp->EditTextureFont(Otp, 10, 255, 255, 255);
								ItemTipTextOtp->ShowTexture(nMouseCoords[0] - 180, nMouseCoords[1] + nYoffset);
							}
							
							if (Item->nStr)
							{
								nYoffset += 15;
								char Strength[10];
								sprintf(Strength, "Strength: %d", Item->nStr);
								ItemTipTextStr->EditTextureFont(Strength, 10, 255, 255, 255);
								ItemTipTextStr->ShowTexture(nMouseCoords[0] - 180, nMouseCoords[1] + nYoffset);
							}
							
							if (Item->nHth)
							{
								nYoffset += 15;
								char Health[10];
								sprintf(Health, "Health: %d", Item->nHth);
								ItemTipTextHealth->EditTextureFont(Health, 10, 255, 255, 255);
								ItemTipTextHealth->ShowTexture(nMouseCoords[0] - 180, nMouseCoords[1] + nYoffset);
							}
							
							if (Item->nAgi)
							{
								nYoffset += 15;
								char Agility[10];
								sprintf(Agility, "Agility: %d", Item->nAgi);
								ItemTipTextAgility->EditTextureFont(Agility, 10, 255, 255, 255);
								ItemTipTextAgility->ShowTexture(nMouseCoords[0] - 180, nMouseCoords[1] + nYoffset);
							}
							
						}
				}
			}
		}

		if (Player->bTalkingToNpc)
		{
			int windowX = WindowSize[0] / 2 - 200;
			int windowY = WindowSize[1] / 2;
			Player->NpcDialogBackground->ShowTexture(windowX, windowY, 400,200);
			Player->NpcDialog->ShowTexture(windowX + 15, windowY + 20);
		}
		
		if (bControlTipsWindow)
			ControlTips->ShowTexture(WindowSize[0] / 2 - 100, WindowSize[1] / 2, 200, 133);
		//Update screen
		SDL_RenderPresent(gRenderer);
		LimitFps->FrameEnd();
	}
}
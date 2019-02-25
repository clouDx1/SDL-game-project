#include <Windows.h>
#include <map>
#include "Monster.h"
#include "Texture.h"
#include "Player.h"

extern std::map <int, MonsterClass*> MonsterMap;
extern PlayerClass* Player;
extern SDL_Renderer* gRenderer;
extern Texture* FlameTexture;


void MonsterDelete(int nID)
{
	auto it = MonsterMap.find(nID);
	if (it != MonsterMap.end())
	{
		it->second->~MonsterClass();
		SAFE_DELETE(it->second)
			MonsterMap.erase(it);
	}
}

void MonsterClass::SetWalk(int nX, int nY, DWORD nTick)
{
	this->WalkRange[0] = nX;
	this->WalkRange[1] = nY;
	this->WalkTick = nTick;
}
void MonsterClass::ChasePlayer()
{
	double fAngle = atan2(Player->Position[1] - this->Position[1], Player->Position[0] - this->Position[0]);
	Position[0] += cos(fAngle) * 3;
	Position[1] += sin(fAngle) * 3;
	this->WalkSleep = GetTickCount() + 50;
}

void MonsterClass::Walk()
{
	if (GetTickCount() > this->WalkSleep)
	{
		double distance = sqrt(pow(this->Position[0] - Player->Position[0], 2) + pow(this->Position[1] - Player->Position[1], 2));
		
		if (GetTickCount() >= TimeSinceLastAtk + 5000)
			AddChaseRange = 0;
		
		if (distance < this->ChanseRange + this->AddChaseRange)
		{
			if (distance > 65)
				this->MonsterClass::ChasePlayer();
			else
				this->MonsterClass::Attack();

			return;
		}

		//do przepisania
		switch (rand() % 4)
		{
		case 0:
			if (Position[0] + 3 < SpawnCoordinates[0] + WalkRange[0] && Position[0] + 3 > SpawnCoordinates[0] - WalkRange[0])
				Position[0] += 3;
			break;
		case 1:
			if (Position[0] - 3 < SpawnCoordinates[0] + WalkRange[0] && Position[0] - 3 > SpawnCoordinates[0] - WalkRange[0])
				Position[0] -= 3;
			break;
		case 2:
			if (Position[1] - 3 < SpawnCoordinates[1] + WalkRange[1] && Position[1] - 3 > SpawnCoordinates[1] - WalkRange[1])
				Position[1] -= 3;
			break;
		case 3:
			if (Position[1] + 3 < SpawnCoordinates[1] + WalkRange[1] && Position[1] - 3 > SpawnCoordinates[1] - WalkRange[1])
				Position[1] += 3;
			break;
		}
		this->WalkSleep = GetTickCount() + 50 + rand() % 150;
		this->bWalking++;
		if (bWalking >= 10)
		{
			this->bWalking = 0;
			this->WalkSleep = GetTickCount() + this->WalkTick;
		}
	}
}
int MonsterClass::Attack()
{
	Player->GetDamage(this->nAttack);
	this->WalkSleep = GetTickCount() + 300;
	this->AttackTimer = GetTickCount() + 300;
	FlameTexture->fAngle = 180 * atan2(Player->Position[1] - this->Position[1], Player->Position[0] - this->Position[0]) / 3.14;
	FlameTexture->SetAnimation(0, 0, 40, 0, 3, 100);
	return 0;
}
int MonsterClass::GetDamage(int nDamage)
{
	if (this->CurrentHp - nDamage > 0)
	{
		if (!this->AddChaseRange) this->WalkSleep = 0;
		
		this->AddChaseRange = 200;
		this->TimeSinceLastAtk = GetTickCount();
		this->CurrentHp -= nDamage;
	}
	else
	{
		Player->AllotExp(this->Experience);
		MonsterDelete(this->nId);
	}
		
}

MonsterClass::~MonsterClass()
{
	this->Textura->~Texture();
	this->Label->~TextureFont();
}
MonsterClass::MonsterClass(int nId,Texture* Textura, TextureFont* Label, int nX, int nY, int nLevel, int nHp, int nMp, int nAttack, int nDefense,int nChaseRange, float fExperience)
{
	this->nId = nId;
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
	this->SpawnCoordinates[0] = nX;
	this->SpawnCoordinates[1] = nY;
	this->AIstate = 1;
	this->ChanseRange = nChaseRange;
	this->AddChaseRange = 0;
	this->Experience = fExperience;
}
#pragma once
#include "Texture.h"
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
void MonsterDelete(int nID);

class MonsterClass
{
public:
	MonsterClass(int nId,Texture* Textura, TextureFont* Label, int nX, int nY, int nLevel, int nHp, int nMp, int nAttack, int nDefense,int nChaseRange,float fExperience);
	~MonsterClass();
	Texture* Textura;
	TextureFont* Label;
	int nId;
	char nLevel;
	char nHp;
	char nMp;
	char CurrentHp;
	char CurrentMp;
	int nAttack;
	int nDefense;
	int Position[2];
	void SetWalk(int nX, int nY, DWORD nTick);
	void Walk();
	void ChasePlayer();
	int bWalking;
	int Attack();
	DWORD WalkTick;
	float Experience;
	int AddChaseRange;
	DWORD WalkSleep;
	DWORD AttackTimer;
	DWORD SpawnCoordinates[2];
	DWORD WalkRange[2];
	DWORD TimeSinceLastAtk;
	char AIstate;
	int ChanseRange;
	int GetDamage(int nDamage);
};

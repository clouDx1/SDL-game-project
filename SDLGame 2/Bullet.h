#pragma once
#include "Texture.h"
#include <map>
class BulletClass;


class BulletClass
{
public:
	char Owner;
	int Position[2];
	int Destination[2];
	int nCurrentSteps;
	int nID;
	int nMaxSteps;
	int nDamage;
	double fAngle;
	void Move();
	void Destroy();
	BulletClass(int nDestination[2], int nPosition[2], int nSteps, int nSpeed, float nAnglePlus);
	~BulletClass();
	Texture* Teksutra;
	int nDeltaX;
	int nDeltaY;
	int Speed;
	DWORD Tick;
};


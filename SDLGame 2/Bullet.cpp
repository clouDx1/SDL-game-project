#include "Bullet.h"
#include <Windows.h>
#include <math.h>
std::map <int, BulletClass*> BulletMap;
extern bool GetCollision(int CollisionType,int nNewCoordinateX, int nNewCoordinateY, int nSizeX, int nSizeY);
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
int nMaxBullet = 0;
extern SDL_Renderer* gRenderer;
extern Texture* Bullet1;
BulletClass::BulletClass(int nDestination[2], int nPosition[2], int nSteps, int nSpeed, float nAnglePlus)
{
	this->nID = nMaxBullet;
	nMaxBullet++;
	this->Destination[0] = nDestination[0];
	this->Destination[1] = nDestination[1];
	this->Position[1] = nPosition[1];
	this->Position[0] = nPosition[0];
	this->nCurrentSteps = 0;
	this->nMaxSteps = nSteps;
	this->Tick = 0;
	this->Speed = nSpeed;
	this->Teksutra = new Texture("Data//Image//Bullet1.png", gRenderer); 
	this->fAngle = atan2(this->Destination[1] - Position[1], this->Destination[0] - Position[0]);
	if (nAnglePlus) this->fAngle += nAnglePlus;
	//this->Teksutra->fAngle = this->fAngle * 180 / 3.141592;
	
	this->Teksutra->SetAnimation(0, 0, 20, 0, 4, 200);
	this->nDeltaX = 10 * cos(this->fAngle);
	this->nDeltaY = 10 * sin(this->fAngle);
	BulletMap.insert(std::pair<int, BulletClass*>(this->nID, this));
}
void BulletClass::Move()
{
	if (GetTickCount() >= this->Tick && this->nCurrentSteps < this->nMaxSteps)
	{
		if (!GetCollision(2, this->Position[0] + this->nDeltaX, this->Position[1] + this->nDeltaY, 15, 15))
		{
				this->Position[0] += this->nDeltaX;
				this->Position[1] += this->nDeltaY;
				this->nCurrentSteps++;
				this->Tick = GetTickCount() + this->Speed;
				this->Speed--;
		}
		else 
			this->BulletClass::Destroy();
	}
	else
		if (this->nCurrentSteps >= this->nMaxSteps)
			this->BulletClass::Destroy();
			
}
void BulletClass::Destroy()
{
	auto it = BulletMap.find(this->nID);
	if (it != BulletMap.end())
	{
		SAFE_DELETE(it->second);
		BulletMap.erase(it);
	}
}

BulletClass::~BulletClass()
{
	
}
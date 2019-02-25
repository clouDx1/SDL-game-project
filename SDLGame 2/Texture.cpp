#pragma once
#include <iostream>
#include <Windows.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "Texture.h"

Texture::Texture(const char* FileName, SDL_Renderer* Renderer)
{
	this->gRenderer = Renderer;
	this->flipType = SDL_FLIP_NONE;
	this->fAngle = 0.0;
	SDL_Surface* loadedSurface = IMG_Load(FileName);
	this->Name = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);
}
Texture::~Texture()
{
	SDL_DestroyTexture(this->Name);
}
void Texture::SetAnimation(int nBeginX, int nBeginY, int nAddX, int nAddY, int nFrames, int nTick)
{
	if (!this->bAnimateDisplay)
	{
		this->bAnimateDisplay = 1;
		this->nAnimationCurrentFrame = 1;
		this->nAminationFrames = nFrames;
		this->nAnimationX = nBeginX;
		this->nAnimationY = nBeginY;
		this->nAnimationAddX = nAddX;
		this->nAnimationAddY = nAddY;
		this->AnimationTimer = GetTickCount() + nTick;
		this->AnimationTick = nTick;
	}
}
bool Texture::Animate(int nX, int nY, int nWidth, int nHeight)
{

	if (this->bAnimateDisplay)
	{
		SDL_Rect DestR;
		SDL_Rect SrcR;
		
		SrcR.x = this->nAnimationX;
		SrcR.y = this->nAnimationY;
		SrcR.w = nWidth;
		SrcR.h = nHeight;

		DestR.x = nX;
		DestR.y = nY;
		DestR.w = nWidth;
		DestR.h = nHeight;

		//SDL_RenderCopy(this->gRenderer, this->Name, &SrcR, &DestR);
		SDL_RenderCopyEx(gRenderer, this->Name, &SrcR, &DestR, this->fAngle, NULL, this->flipType);
		if (GetTickCount() > AnimationTimer)
		{
			if (this->nAnimationCurrentFrame < this->nAminationFrames)
			{
				SDL_RenderCopyEx(gRenderer, this->Name, &SrcR, &DestR, this->fAngle, NULL, this->flipType);
				this->nAnimationCurrentFrame++;
				this->nAnimationX += this->nAnimationAddX;
				this->nAnimationY += this->nAnimationAddY;
				this->AnimationTimer = GetTickCount() + this->AnimationTick;
			}
			else
			{
				this->bAnimateDisplay = 0;
				this->nAnimationCurrentFrame = 0;
				this->nAminationFrames = 0;
				this->nAnimationX = 0;
				this->nAnimationY = 0;
				this->nAnimationAddX = 0;
				this->nAnimationAddY = 0;
				this->AnimationTimer = 0;
				this->AnimationTick = 0;
			}
		}
		return true;
	}

	return false;
}
void Texture::ShowAnimatedTexture(int nX, int nY, int nWidth, int nHeight,int nBeginX, int nBeginY)
{
	if (this->Animate(nX, nY, nWidth, nHeight))
		return;
	
	SDL_Rect SrcR;

	SrcR.x = nBeginX;
	SrcR.y = nBeginY;
	SrcR.w = nWidth;
	SrcR.h = nHeight;

	SDL_Rect DestR;
	DestR.x = nX;
	DestR.y = nY;
	DestR.w = nWidth;
	DestR.h = nHeight;
	SDL_RenderCopyEx(gRenderer, this->Name, &SrcR, &DestR, this->fAngle, NULL, this->flipType);
}
void Texture::ShowTexture(int nX, int nY, int nWidth, int nHeight)
{
	SDL_Rect DestR;
	DestR.x = nX;
	DestR.y = nY;
	DestR.w = nWidth;
	DestR.h = nHeight;
	SDL_RenderCopyEx(gRenderer, this->Name, NULL, &DestR, this->fAngle, NULL, this->flipType);
}

TextureFont::TextureFont(const char* FileName, SDL_Renderer* Renderer, const char* Text, int nSize, Uint8 nRed, Uint8 nGreen, Uint8 nBlue)
{
		this->gRenderer = Renderer;
		this->Font = TTF_OpenFont(FileName, nSize);
		this->Kolor = { nRed, nGreen, nBlue };
		TTF_SizeText(this->Font, Text, &nTextSize[0], &nTextSize[1]);
		SDL_Surface* loadedSurface = TTF_RenderText_Blended(this->Font, Text, this->Kolor);
		this->Name = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
		SDL_FreeSurface(loadedSurface);
}
TextureFont::~TextureFont()
{
	SDL_DestroyTexture(this->Name);
}

void TextureFont::ShowTexture(int nX, int nY)
{
		SDL_Rect DestR;
		DestR.x = nX;
		DestR.y = nY;
		DestR.w = nTextSize[0];
		DestR.h = nTextSize[1];
		SDL_RenderCopy(this->gRenderer, this->Name, NULL, &DestR);
}


void TextureFont::EditTextureFont(const char* szNewText, int nSize, Uint8 nRed, Uint8 nGreen, Uint8 nBlue)
{
	this->Kolor = { nRed, nGreen, nBlue };
	TTF_SizeText(this->Font, szNewText, &nTextSize[0], &nTextSize[1]);
	SDL_Surface* loadedSurface = TTF_RenderText_Blended(this->Font, szNewText, this->Kolor);
	SDL_DestroyTexture(this->Name);
	this->Name = SDL_CreateTextureFromSurface(this->gRenderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);
}

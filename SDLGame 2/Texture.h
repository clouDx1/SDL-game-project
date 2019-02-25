#pragma once

#include <Windows.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

class Texture
{
public:
	SDL_Texture *Name;
	SDL_Renderer *gRenderer;
	DWORD Tick;

	bool bAnimateDisplay;
	int nAnimationCurrentFrame;
	int nAminationFrames;
	int nAnimationAddX;
	int nAnimationAddY;
	int nAnimationX;
	int nAnimationY;
	DWORD AnimationTimer;
	DWORD AnimationTick;

	SDL_RendererFlip flipType;
	float fAngle;

	~Texture();
	Texture(const char* FileName, SDL_Renderer* Renderer);
	void ShowTexture(int nX, int nY, int nWidth, int nHeight);
	void ShowAnimatedTexture(int nX, int nY, int nWidth, int nHeight, int nBeginX, int nBeginY);
	bool Animate(int nX, int nY, int nWidth, int nHeight);
	void SetAnimation(int nBeginX, int nBeginY, int nAddX, int nAddY, int nFrames, int nTick);

};

class TextureFont
{
public:
	SDL_Texture *Name;
	SDL_Renderer *gRenderer;
	DWORD Tick;
	TTF_Font* Font;
	SDL_Color Kolor;
	int nTextSize[2];

	~TextureFont();
	TextureFont(const char* FileName, SDL_Renderer* Renderer, const char* Text, int nSize, Uint8 nRed, Uint8 nGreen, Uint8 nBlue);
	void EditTextureFont(const char* szNewText, int nSize, Uint8 nRed, Uint8 nGreen, Uint8 nBlue);
	void ShowTexture(int nX, int nY);
};

#pragma once

#include <Windows.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

class Surface
{
public:
	SDL_Surface *Name;
	TTF_Font* Font;
	SDL_Color Kolor;
	int nTextSize[2];
	DWORD Tick;

	~Surface();
	Surface(const char* FileName, const char* Text, int nSize, Uint8 nRed, Uint8 nGreen, Uint8 nBlue);
	void EditText(const char* szNewText, int nSize, Uint8 nRed, Uint8 nGreen, Uint8 nBlue);
	void ShowSurface(SDL_Surface* Window, int nX, int nY);
	
};

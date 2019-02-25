#pragma once
#include <Windows.h>
#include <time.h>
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

class Fps
{
public:
	char FpsMax;
	DWORD Begin;
	DWORD End;
	float Delay;
	Fps(int nFpsMax);
	void FrameBegin();
	void FrameEnd();
};

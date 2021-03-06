#include <iostream>
#include <Windows.h>
#include <time.h>
//<--- SDL INCLUDE
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
//<-- Game Includes
#include "MainGame.h"
#include "MainMenu.h"
#include "MainOptions.h"
#include "Surface.h"

#undef main

int WindowSize[2];
int nMouseCoords[2];
SDL_Window* Window;
SDL_Surface* Ekran;
SDL_Surface* EkranGracza;
SDL_Cursor* Cursor;
SDL_Event Event;
SDL_Renderer* gRenderer;
DWORD WalkSleep;
DWORD AttackSleep;
DWORD NpcTalkSleep;
char GameState = 0;

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init(); // <-- jezeli chcemy uzywac czcione to trzeba ja zainicjowac
	IMG_Init(IMG_INIT_PNG); // <-- Load Pngs
	
	
	Cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
	SDL_SetCursor(Cursor);
	
	// Jezeli chcemy full screen window to na koncu dodajemy event
	Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 600, 0);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); //Set texture filtering to linear
	gRenderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

	Ekran = SDL_GetWindowSurface(Window);

	SDL_GetWindowSize(Window, &WindowSize[0], &WindowSize[1]);
	printf("[DEBUG] Window Created width[%d] height[%d].\n", WindowSize[0], WindowSize[1]);
	
	while (true)
	{
		if (GameState == 0) // Main Menu
			CreateGameMenu();
		if (GameState == 1) // Main Game
			CreateGame();
		if (GameState == 2) // Options
			CreateMainOptions();
		if (GameState == 3) // Game Quit
			return 1;
	}
}


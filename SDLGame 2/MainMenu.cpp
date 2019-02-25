#include "MainMenu.h"
#include "Texture.h"
#include "Fps.h"

extern int WindowSize[2];
extern SDL_Window* Window;
extern SDL_Surface* Ekran;
extern SDL_Event Event;
extern DWORD WalkSleep;
extern char GameState;
extern SDL_Renderer* gRenderer;

int LabelPos[2] = { 400,400 };

void CreateGameMenu()
{
	Fps* LimitFps = new Fps(60);
	Texture* Block1 = new Texture("Data//Image//Block1.png", gRenderer); // wall
	Texture* Block2 = new Texture("Data//Image//Block2.png", gRenderer); // grass
	Texture* Darkness = new Texture("Data//Image//Darkness.png", gRenderer); // darkness
	TextureFont* NewGame = new TextureFont("Data//Font/8-BIT WONDER.ttf", gRenderer, "New Game", 40, 255, 255, 255);
	TextureFont* Options = new TextureFont("Data//Font//8-BIT WONDER.ttf", gRenderer, "Options", 40, 255, 255, 255);
	TextureFont* Exit = new TextureFont("Data//Font//8-BIT WONDER.ttf", gRenderer, "Exit", 40, 255, 255, 255);
	TextureFont* Info = new TextureFont("Data//Font//VCR_OSD_MONO_1.001.ttf", gRenderer, "Game created by clouDx 2019", 30, 255, 255, 255);
	TextureFont* Version = new TextureFont("Data//Font//VCR_OSD_MONO_1.001.ttf", gRenderer, "Version 1.0.0", 25, 0, 0, 0);
	char MenuSelect = 0;
	bool Changed = true;
	int MenuPos[2] = { 0,0 };
	DWORD MenuTick = 0;

	while (GameState == 0)
	{
		LimitFps->FrameBegin();
		SDL_RenderClear(gRenderer);
		for (int i = 0; i < WindowSize[0]; i += 40)
		{
			for (int y = 0; y < WindowSize[1]; y += 40)
			{
				if (i == 0 || y == 0 || y == WindowSize[1] - 40 || i == WindowSize[0] - 40)
					Block1->ShowTexture(i, y, 40, 40);
				else
					Block2->ShowTexture(i, y, 40, 40);
			}

		}

		if (Changed)
		{
			Changed = false;
			switch (MenuSelect)
			{
			case 0:
				NewGame->EditTextureFont("New Game", 40, 255, 0, 0);
				Options->EditTextureFont("Options", 40, 255, 255, 255);
				Exit->EditTextureFont("Exit", 40, 255, 255, 255);
				MenuPos[0] = 150; MenuPos[1] = 140;
				break;
			case 1:
				NewGame->EditTextureFont("New Game", 40, 255, 255, 255);
				Options->EditTextureFont("Options", 40, 255, 0, 0);
				Exit->EditTextureFont("Exit", 40, 255, 255, 255);
				MenuPos[0] = 150; MenuPos[1] = 200;
				break;
			case 2:
				NewGame->EditTextureFont("New Game", 40, 255, 255, 255);
				Options->EditTextureFont("Options", 40, 255, 255, 255);
				Exit->EditTextureFont("Exit", 40, 255, 0, 0);
				MenuPos[0] = 150; MenuPos[1] = 260;
				break;
			}
		}


		NewGame->ShowTexture(150, 140);
		Options->ShowTexture(150, 200);
		Exit->ShowTexture(150, 260);


		while (SDL_PollEvent(&Event)) //esc?
		{
			if (Event.type == SDL_QUIT)
				GameState = 3;
		}

		// <---------------------- Darkne$$$
		int Radius = 50;
		for (int ni = 0; ni < WindowSize[0]; ni += 5)
		{
			for (int ny = 0; ny < WindowSize[1]; ny += 5)
			{
				if ((ny < MenuPos[1] || ny > MenuPos[1] + 50) || (ni < MenuPos[0] - 10 || ni > MenuPos[0] + 400) || (rand() % 10 < 3))
				{
					
					SDL_SetTextureAlphaMod(Darkness->Name, 70);
					Darkness->ShowTexture(ni, ny, 5, 5);
				}

			}
		}
		Version->ShowTexture(50, 500);
		Info->ShowTexture(50, 530);
	

		//<-- MENU SELECTION
		if (GetTickCount() > MenuTick)
			if (GetAsyncKeyState(0x57)) // <--- 'W' Key
			{
				if (MenuSelect >= 1)
				{
					MenuSelect -= 1;
					MenuTick = GetTickCount() + 200;
					Changed = true;
				}
			}
			else if (GetAsyncKeyState(0x53)) // <--- 'S' Key
			{
				if (MenuSelect <= 2)
				{
					MenuSelect += 1;
					MenuTick = GetTickCount() + 200;
					Changed = true;
				}
			}
			else if (GetAsyncKeyState(0x0D)) // <--- 'Enter' Key
			{
				switch (MenuSelect)
				{
					case 0:
						GameState = 1;
					break;
					case 1:
						GameState = 2;
					break;
					case 2:
						GameState = 3;
					break;
				}
			}


		SDL_RenderPresent(gRenderer);
		LimitFps->FrameEnd();
	}
	Block1->~Texture();
	Block2->~Texture();
}


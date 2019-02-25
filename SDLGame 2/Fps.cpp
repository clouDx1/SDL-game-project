#include "Fps.h"
extern SDL_Window* Window;
char FpsShow[30];
int Frame = 0;
int FpsCountTimer = 0;
Fps::Fps(int nFpsMax)
{
	FpsCountTimer = clock();
	Frame = 0;
	this->FpsMax = nFpsMax;
	printf("[DEBUG] Fps set: %d.\n", this->FpsMax);
	this->Begin = 0;
	this->End = 0;
}
void Fps::FrameBegin()
{
	this->Begin = clock();
}

void Fps::FrameEnd()
{	
	Frame++;
	sprintf(FpsShow, "Game FPS %.2f", 1000 * (double) Frame / (clock() - FpsCountTimer));
	
	if ((1000.0 / this->FpsMax) - (clock() - this->Begin) > 0)
	{
		SDL_Delay((1000.0 / this->FpsMax) - (clock() - this->Begin));
		SDL_SetWindowTitle(Window, FpsShow);
	}	
}
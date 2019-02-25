#include "Surface.h"
extern SDL_Surface* Ekran;

Surface::~Surface()
{
	if(this->Name)
		SDL_FreeSurface(this->Name);
}

Surface::Surface(const char* FileName, const char* Text, int nSize, Uint8 nRed, Uint8 nGreen, Uint8 nBlue)
{
	if (!Text)
	{
		SDL_Surface* Temporary = IMG_Load(FileName);
		this->Name = SDL_ConvertSurface(Temporary, Ekran->format, NULL);
		SDL_FreeSurface(Temporary);
	}	
	else
	{
		this->Font = TTF_OpenFont(FileName, nSize);
		this->Kolor = { nRed, nGreen, nBlue };
		TTF_SizeText(this->Font, Text, &nTextSize[0], &nTextSize[1]);
		this->Name = TTF_RenderText_Blended(this->Font, Text, this->Kolor);

	}
}

void Surface::EditText(const char* szNewText, int nSize, Uint8 nRed, Uint8 nGreen, Uint8 nBlue)
{
	this->Kolor = { nRed, nGreen, nBlue };
	TTF_SizeText(this->Font, szNewText, &nTextSize[0], &nTextSize[1]);
	SDL_FreeSurface(Name); //<-- Memory Leak bez tego ;o
	this->Name = TTF_RenderText_Blended(this->Font, szNewText, this->Kolor);
}

void Surface::ShowSurface(SDL_Surface* Window, int nX, int nY)
{
	SDL_Rect Rect;
	Rect.x = nX;
	Rect.y = nY;
	SDL_BlitSurface(this->Name, NULL, Window, &Rect);
}
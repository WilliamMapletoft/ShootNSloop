#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
class Ending
{
private:
	SDL_Surface* timerText = NULL;

	SDL_Texture* timerTexture = NULL;

	SDL_Rect destText;
	void LoadText(TTF_Font* gameFont, SDL_Renderer* passedRenderer);
	const char* textToDisplay;
	SDL_Color colour = { 255, 255, 255 };
	int w, h;
	bool done;
	SDL_Event _event;
public:
	void Display(bool playerWin, SDL_Renderer* passedRenderer, TTF_Font* gameFont, int W, int H);
	void Render(SDL_Renderer* passedRenderer);
};


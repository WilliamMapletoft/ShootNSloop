#include "Ending.h"

void Ending::Display(bool playerWin, SDL_Renderer* passedRenderer, TTF_Font* gameFont, int w, int h) {

	if (playerWin == true) {
		textToDisplay = "Congratulations!";
	}
	else
	{
		textToDisplay = "Better luck next time!";
	}

	LoadText(gameFont, passedRenderer);
	destText.w = 900;
	destText.h = 150;

	destText.x = (w / 2) - (destText.w / 2);
	destText.y = (h / 2) - (destText.h / 2);
	while (done != true) {
		Render(passedRenderer);

        while (SDL_PollEvent(&_event))
        {

            if (_event.type == SDL_QUIT) { done = true; }

			if (_event.type == SDL_KEYDOWN && _event.key.repeat == NULL)
			{
				switch (_event.key.keysym.sym) {
				case SDLK_ESCAPE:
					done = true;
					break;
				}
			}
        }
	}
}

void Ending::Render(SDL_Renderer* passedRenderer) {

	SDL_SetRenderDrawColor(passedRenderer, 15, 15, 15, 255);

	SDL_RenderClear(passedRenderer);

	SDL_RenderCopy(passedRenderer, timerTexture, NULL, &destText);

	SDL_RenderPresent(passedRenderer);
}

void Ending::LoadText(TTF_Font* gameFont, SDL_Renderer* passedRenderer) {

	timerText = TTF_RenderText_Solid(gameFont, textToDisplay, colour);

	timerTexture = SDL_CreateTextureFromSurface(passedRenderer, timerText);
}
#include <iostream>
#include "SDL.h"
#include "GameLoop.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

std::vector<const char*> logs;

int main(int argc, char* argv[])
{

	GameLoop mainLoop;

	mainLoop.Update();

	return 0;
}
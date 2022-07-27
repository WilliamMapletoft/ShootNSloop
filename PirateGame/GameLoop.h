#pragma once
#include <SDL.h>
#include "SDL_mixer.h"
#include "SoundController.h"
#include "GameObject.h"
#include "SDL_image.h"
#include "Timer.h"
#include <SDL_ttf.h>
#include <string>
#include "Ending.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstddef>

class GameLoop
{
public:
	bool done = false;
	void Update();
	void Render(SDL_Renderer* passedRenderer);
	void MainMenu();
	void LoadUI();
private:
	void CollisionDetect();
};


#pragma once
#include <SDL.h>
#include "SDL_mixer.h"

class SoundController
{
private:
	int currentMusicVol = 128;
	int currentVol = 128;
	Mix_Music* BackGroundMusic = NULL;
	Mix_Chunk* shot = NULL;
public:
	SoundController();
	void Init();
	void Pause();
	void ChangeVolume(int change);
	void ChangeMusic(int change);
	void ChangeFX(int change);
	void Remove();
	void PlayCShot();
};


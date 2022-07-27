#include "SoundController.h"
#include <SDL.h>
#include "SDL_mixer.h"

SoundController::SoundController() {

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load SDL_Mixer library");
	}
	{
		BackGroundMusic = Mix_LoadMUS("./content/music/Miri.mp3");
		Mix_PlayMusic(BackGroundMusic, -1);
		shot = Mix_LoadWAV("content/sounds/cannon.wav");
		if (shot != NULL) {
			SDL_Log("Shot loaded successfully");
		}
		else
		{
			SDL_Log("Shot failed to load");
		}
	}
}

void SoundController::Init() {
	
}

void SoundController::Pause() {
	if (Mix_PlayingMusic()) {
		Mix_HaltMusic();
	}
	else
	{
		Mix_PlayMusic(BackGroundMusic, -1);
	}
}

void SoundController::ChangeVolume(int change) {
	if (currentVol > 0 && change < 0) {
		currentVol += change;
	}
	else if (currentVol < 128 && change > 0) {
		currentVol += change;
	}
	Mix_Volume(-1, (currentVol));
	if (currentMusicVol > 0 && change < 0) {
		currentMusicVol += change;
	}
	else if (currentMusicVol < 128 && change > 0) {
		currentMusicVol += change;
	}
	Mix_VolumeMusic(currentMusicVol);
	SDL_Log("Current volume : %i", currentVol);
	SDL_Log("Current music volume : %i", currentMusicVol);
}

void SoundController::ChangeMusic(int change) {
	if (currentMusicVol > 0 && change < 0) {
		currentMusicVol += change;
	}
	else if (currentMusicVol < 128 && change > 0) {
		currentMusicVol += change;
	}
	Mix_VolumeMusic(currentMusicVol);
	SDL_Log("Current music volume : %i", currentMusicVol);
}

void SoundController::ChangeFX(int change) {
	if (currentVol > 0 && change < 0) {
		currentVol += change;
	}
	else if (currentVol < 128 && change > 0) {
		currentVol += change;
	}
	Mix_Volume(-1, (currentVol));
	SDL_Log("Current volume : %i", currentVol);
}
void SoundController::Remove() {
	SDL_Log("Audio manager removed");
	Mix_CloseAudio();
	Mix_FreeChunk(shot);
	Mix_FreeMusic(BackGroundMusic);
}

void SoundController::PlayCShot() {
	Mix_PlayChannel(-1, shot, 0);
}
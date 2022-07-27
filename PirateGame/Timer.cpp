#include "Timer.h"

Timer::Timer() {
	startTicks = 0;
}

void Timer::resetTicksTimer() {
	startTicks = SDL_GetTicks();
}

int Timer::getTicks() {
	return(SDL_GetTicks() - startTicks);
}
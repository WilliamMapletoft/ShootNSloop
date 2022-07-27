#pragma once
#include <SDL.h>
#include <vector>
#include "Timer.h"
#include <SDL.h>
#include "SDL_image.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "SoundController.h"

class GameObject
{
public:
};

class CannonBall : public GameObject {
protected:
	virtual void CreateTexture(SDL_Renderer* passedRenderer);
	SDL_Point center = { 3, 3 };
	Timer lifeTime;
	int lifeDuration;
	double posX, posY;
public:
	SDL_Surface* cannonBallSurf = NULL;
	SDL_Texture* cannonBallTexture = NULL;
	SDL_Rect sourceRect, destRect;
	double velocityX, velocityY;
	bool active;
	virtual void Init(SDL_Renderer* passedRenderer);
	void Update();
	void Render(SDL_Renderer* passedRenderer);
	void SetPosition(SDL_Point spawnLocation);
};

class FireBomb : public CannonBall {
public:
	int angleMod;
	void CreateTexture(SDL_Renderer* passedRenderer);
	void Init(SDL_Renderer* passedRenderer, int angle);
};

class Bomb : public GameObject{
private:
	Timer lifeTime;
	int lifeDuration;
public:
	SDL_Surface* bombSurf = NULL;
	SDL_Texture* bombTexture = NULL;
	SDL_Rect sourceRect, destRect;
	double posX, posY;
	bool active, blown;
	void CreateTexture(SDL_Renderer* passedRenderer);
	void Init(SDL_Renderer* passedRenderer);
	void Render(SDL_Renderer* passedRenderer);
	void Update();
	void SetPosition(SDL_Point spawnLocation);
};

class ShipObject : public GameObject
{
protected:
	void calculateSailWind(const char* wind);
	double sailMod;
	SDL_Rect healthBar, crewHealthBar, sourceShipRect, sourceSailRect;
	int shipClock;
	int trueAngle;
	SDL_Surface* shipSurf = NULL;
	SDL_Texture* shipTexture = NULL;
	SDL_Surface* sailSurf = NULL;
	SDL_Texture* sailTexture = NULL;
	virtual void LoadTextures(SDL_Renderer* passedRenderer);
	std::vector<const char*> logData;
public:
	double shipX, shipY;
	SDL_Point frontLeft, frontRight, rearLeft, rearRight;
	std::vector<FireBomb> fireBombs;
	CannonBall cannonBall;
	Bomb bomb;
	int health, crewHealth, ballSelected;
	ShipObject(SDL_Renderer* passedRenderer);
	ShipObject();
	SDL_Rect sailRect;
	SDL_Rect shipRect;
	int sailR, sailG, sailB, shipR, shipG, shipB;
	double angle = 0;
	double sailAngle = 0;
	bool turnL, turnR, sTurnL, sTurnR;
	int sailSetting = 0;
	SDL_Point center = {50, 50};
	SDL_Point centerSail = { 25, 25 };
	void Render(SDL_Renderer* passedRenderer);
	virtual void Update(int width, int height, const char* wind);
	void UpdateSail();
	void FireCannon(bool direction);
	std::vector<const char*> sendLog();
};

class WindCompass : public GameObject
{
private:
	SDL_Surface* compassSurf = NULL;
	SDL_Texture* compassTexture = NULL;
	SDL_Surface* arrowSurf = NULL;
	SDL_Texture* arrowTexture = NULL;
	SDL_Rect windSource, windDest;
	void createTextures(SDL_Renderer* passedRenderer);
public:
	const char* cardinal;
	int windAngle;
	WindCompass(SDL_Renderer* passedRenderer);
	void Render(SDL_Renderer* passedRenderer);
};

class EnemyShip : public ShipObject {
private:
	void LoadTextures(SDL_Renderer* passedRenderer);
public:
	int shootMode;
	void Update(int width, int height, const char* wind, SoundController* aM);
	double s1X, s1Y, s2X, s2Y;
	EnemyShip(SDL_Renderer* passedRenderer);
};
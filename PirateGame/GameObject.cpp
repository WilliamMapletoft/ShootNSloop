#include "GameObject.h"

// CANNONBALL CODE

void CannonBall::Init(SDL_Renderer* passedRenderer) {
	CreateTexture(passedRenderer);
	sourceRect.x = 0;
	sourceRect.y = 0;
	sourceRect.w = 2;
	sourceRect.h = 2;

	destRect.x = 0;
	destRect.y = 0;
	destRect.w = 6;
	destRect.h = 6;

	lifeDuration = 1250;

	active = false;
}

void CannonBall::CreateTexture(SDL_Renderer* passedRenderer) {
	cannonBallSurf = IMG_Load("content/art/cannonball.png");
	if (cannonBallSurf == NULL)
	{
		SDL_Log("Null pointer returned while loading cannonball.png");
	}
	{
		SDL_Log("Loaded cannonball.png successfully");
	}

	cannonBallTexture = SDL_CreateTextureFromSurface(passedRenderer, cannonBallSurf);
	if (cannonBallTexture == NULL)
	{
		SDL_Log("Null pointer returned while converting surface to texture");
	}
	{
		SDL_Log("cannonBallTexture created successfully");
		SDL_FreeSurface(cannonBallSurf);
	}
}

void CannonBall::Update() {
	if (lifeTime.getTicks() < lifeDuration) {
		//SDL_Log("Timer : %i", lifeTime.getTicks());
		posX = posX + velocityX;
		posY = posY - velocityY;
		destRect.x = round(posX);
		destRect.y = round(posY);
	}
	else
	{
		velocityX = 0;
		velocityY = 0;
		active = false;
	}

}

void CannonBall::Render(SDL_Renderer* passedRenderer) {
	//SDL_RenderFillRect(passedRenderer, &destRect);
	//SDL_Log("destRect X : %i Y : %i", destRect.x, destRect.y);
	//SDL_Log("destRect W : %i H : %i", destRect.w, destRect.h);
	SDL_RenderCopyEx(passedRenderer, cannonBallTexture, &sourceRect, &destRect, 0, &center, SDL_FLIP_NONE);
}

void CannonBall::SetPosition(SDL_Point spawnLocation) {
	posX = spawnLocation.x - (destRect.w / 2);
	posY = spawnLocation.y - (destRect.h / 2);
	lifeTime.resetTicksTimer();
}

// FIREBOMB CODE

void FireBomb::Init(SDL_Renderer* passedRenderer, int angle) {
	CreateTexture(passedRenderer);
	sourceRect.x = 0;
	sourceRect.y = 0;
	sourceRect.w = 2;
	sourceRect.h = 2;

	destRect.x = 0;
	destRect.y = 0;
	destRect.w = 6;
	destRect.h = 6;

	lifeDuration = 750;

	angleMod = angle;

	//SDL_Log("Angle : %i", angle);

	active = false;
}

void FireBomb::CreateTexture(SDL_Renderer* passedRenderer) {
	cannonBallSurf = IMG_Load("content/art/firebomb.png");
	if (cannonBallSurf == NULL)
	{
		SDL_Log("Null pointer returned while loading firebomb.png");
	}
	{
		SDL_Log("Loaded firebomb.png successfully");
	}

	cannonBallTexture = SDL_CreateTextureFromSurface(passedRenderer, cannonBallSurf);
	if (cannonBallTexture == NULL)
	{
		SDL_Log("Null pointer returned while converting surface to texture");
	}
	{
		SDL_Log("cannonBallTexture created successfully");
		SDL_FreeSurface(cannonBallSurf);
	}
}

// BOMB CODE

void Bomb::Init(SDL_Renderer* passedRenderer) {
	CreateTexture(passedRenderer);

	sourceRect.x = 0;
	sourceRect.y = 0;
	sourceRect.w = 4;
	sourceRect.h = 4;

	destRect.x = 0;
	destRect.y = 0;
	destRect.w = 8;
	destRect.h = 8;

	lifeDuration = 2000;
}

void Bomb::CreateTexture(SDL_Renderer* passedRenderer) {
	bombSurf = IMG_Load("content/art/Bomb.png");
	if (bombSurf == NULL)
	{
		SDL_Log("Null pointer returned while loading Bomb.png");
	}
	{
		SDL_Log("Loaded Bomb.png successfully");
	}

	bombTexture = SDL_CreateTextureFromSurface(passedRenderer, bombSurf);
	if (bombTexture == NULL)
	{
		SDL_Log("Null pointer returned while converting surface to texture");
	}
	{
		SDL_Log("bombTexture created successfully");
		SDL_FreeSurface(bombSurf);
	}
}

void Bomb::SetPosition(SDL_Point spawnLocation) {
	posX = spawnLocation.x - (destRect.w / 2);
	posY = spawnLocation.y - (destRect.h / 2);

	destRect.x = round(posX);
	destRect.y = round(posY);

	lifeTime.resetTicksTimer();
}

void Bomb::Update() {
	if (lifeTime.getTicks() > lifeDuration) {
		blown = true;
	}
	if (lifeTime.getTicks() > lifeDuration + 100) {
		active = false;
		blown = false;
	}
}

void Bomb::Render(SDL_Renderer* passedRenderer) {
	SDL_RenderCopyEx(passedRenderer, bombTexture, &sourceRect, &destRect, 0, new SDL_Point{4, 4}, SDL_FLIP_NONE);
	//SDL_RenderFillRect(passedRenderer, &destRect);
}

// SHIP CODE

ShipObject::ShipObject(SDL_Renderer* passedRenderer) {

	LoadTextures(passedRenderer);

	sourceShipRect.h = 32;
	sourceShipRect.w = 32;
	sourceShipRect.x = sourceShipRect.w * sailSetting;
	sourceShipRect.y = 0;

	sourceSailRect.h = 16;
	sourceSailRect.w = 16; 
	sourceSailRect.x = sourceSailRect.w * sailSetting;
	sourceSailRect.y = 0;

	sailR = 255;
	sailG = 255;
	sailB = 255;
	shipR = 99;
	shipG = 67;
	shipB = 39;
	sailRect.w = 50;
	sailRect.h = 50;
	shipRect.w = 100;
	shipRect.h = 100;

	shipRect.x = 50;
	shipX = 50.0;
	shipRect.y = 50;
	shipY = 50.0;

	health = 100;
	crewHealth = 100;

	cannonBall.Init(passedRenderer);
	for (int i = 0; i < 3; i++) {
		FireBomb tempBomb;
		tempBomb.Init(passedRenderer, ((i - 1) * 15));
		fireBombs.push_back(tempBomb);
	}
	bomb.Init(passedRenderer);
}

ShipObject::ShipObject() {
	SDL_Log("Empty constructor called");
}

void ShipObject::Update(int width, int height, const char* wind) {

	if (turnL == true) {
		angle = angle - (2 - sailSetting / 2);
	}
	if (turnR == true) {
		angle = angle + (2 - sailSetting / 2);
	}

	trueAngle = abs(fmod(angle, 360) + (angle / 360));

	

	if (sTurnL == true) {
		sailAngle = sailAngle - 2;
	}
	if (sTurnR == true) {
		sailAngle = sailAngle + 2;
	}
	if (angle - sailAngle > 90)
	{
		sailAngle = angle - 90;
	}
	else if (sailAngle - angle > 90)
	{
		sailAngle = angle + 90;
	}

	bool moveThisTurn = true;

	if ((frontLeft.x + (sin(angle * M_PI / 180))) > width+40 || (frontRight.x + (sin(angle * M_PI / 180))) > width + 40) {
		shipRect.x = -40;
		shipX = -40;
		SDL_Log("Ship exited map right and was reset");
	}
	else if ((frontLeft.x - (sin(angle * M_PI / 180))) < -40 || (frontRight.x - (sin(angle * M_PI / 180))) < -40) {
		shipRect.x = width - 40;
		shipX = width - 40;
		SDL_Log("Ship exited map left and was reset");
	}
	if ((frontLeft.y + (cos(angle * M_PI / 180))) > height+40 || (frontRight.y + (cos(angle * M_PI / 180))) > height + 40) {
		shipRect.y = -40;
		shipY = -40;
		SDL_Log("Ship exited map down and was reset");
	}
	else if ((frontLeft.y - (cos(angle * M_PI / 180))) < -40 || (frontRight.y - (cos(angle * M_PI / 180))) < -40) {
		shipRect.y = height - 40;
		shipY = height - 40;
		SDL_Log("Ship exited map up and was reset");
	}


	if (moveThisTurn != false) {

		calculateSailWind(wind);

		shipY = shipY - ((cos(angle * M_PI / 180) * sailSetting) * sailMod);
		shipX = shipX + ((sin(angle * M_PI / 180) * sailSetting) * sailMod);

		shipRect.y = round(shipY);
		shipRect.x = round(shipX);
	}

	sailRect.x = (shipRect.x + (shipRect.w / 2)) - (sailRect.w / 2);
	sailRect.y = (shipRect.y + (shipRect.h / 2)) - (sailRect.h / 2);

	shipClock = SDL_GetTicks();

	//SDL_Log("Angle : %i", trueAngle);
	
	frontRight.y = shipY - (cos((angle + 22) * M_PI / 180) * 33) + 50;
	frontRight.x = shipX + (sin((angle + 22) * M_PI / 180) * 33) + 50;

	frontLeft.y = shipY - (cos((angle - 22) * M_PI / 180) * 33) + 50;
	frontLeft.x = shipX + (sin((angle - 22) * M_PI / 180) * 33) + 50;

	rearRight.y = shipY + (cos((angle - 22) * M_PI / 180) * 33) + 50;
	rearRight.x = shipX - (sin((angle - 22) * M_PI / 180) * 33) + 50;
	
	rearLeft.y = shipY + (cos((angle + 22) * M_PI / 180) * 33) + 50;
	rearLeft.x = shipX - (sin((angle + 22) * M_PI / 180) * 33) + 50;

	healthBar.x = shipX + (50 - (health / 4));
	healthBar.y = shipY + 15;
	healthBar.w = health / 2;
	healthBar.h = 2;

	crewHealthBar.x = shipX + (50 - (crewHealth / 4));
	crewHealthBar.y = shipY + 17;
	crewHealthBar.w = crewHealth / 2;
	crewHealthBar.h = 2;

}

void ShipObject::Render(SDL_Renderer* passedRenderer)
{
	
	//SDL_RenderDrawRect(passedRenderer, &shipRect);

	SDL_SetRenderDrawColor(passedRenderer, 255, 0, 0, 255);

	SDL_RenderCopyEx(passedRenderer, shipTexture, &sourceShipRect, &shipRect, angle, &center, SDL_FLIP_NONE);

	SDL_RenderCopyEx(passedRenderer, sailTexture, &sourceSailRect, &sailRect, sailAngle, &centerSail, SDL_FLIP_NONE);

	SDL_RenderFillRect(passedRenderer, &healthBar);

	SDL_SetRenderDrawColor(passedRenderer, 0, 255, 0, 255);

	SDL_RenderDrawRect(passedRenderer, &crewHealthBar);

	if (cannonBall.active == true) {
		cannonBall.Render(passedRenderer);
	}

	for (int i = 0; i < 3; i++) {
		if (fireBombs[i].active == true) {
			fireBombs[i].Render(passedRenderer);
		}
	}

	if (bomb.active == true) {
		bomb.Render(passedRenderer);
	}

	/*SDL_RenderDrawLine(passedRenderer, frontRight.x, frontRight.y, rearRight.x, rearRight.y);
	SDL_RenderDrawLine(passedRenderer, frontLeft.x, frontLeft.y, rearLeft.x, rearLeft.y);
	SDL_RenderDrawLine(passedRenderer, frontRight.x, frontRight.y, frontLeft.x, frontLeft.y);
	SDL_RenderDrawLine(passedRenderer, rearRight.x, rearRight.y, rearLeft.x, rearLeft.y);*/

	SDL_SetRenderDrawColor(passedRenderer, 255, 255, 255, 255);
	//SDL_RenderDrawRect(passedRenderer, &sailRect);
}

void ShipObject::UpdateSail() {
	sourceShipRect.x = sourceShipRect.w * sailSetting;
	sourceSailRect.x = sourceSailRect.w * sailSetting;
}

void ShipObject::calculateSailWind(const char* wind) {
	int trueSailAngle = fmod(sailAngle, 360) + (sailAngle / 360);
	if (trueSailAngle < 0)
	{
		trueSailAngle = 360 + trueSailAngle;
	}

	const char* sailCardinal;
	
	if (trueSailAngle < 23) {
		sailCardinal = "North";
	}
	else if (trueSailAngle < 68){
		sailCardinal = "NorthEast";
	}
	else if (trueSailAngle < 113) {
		sailCardinal = "East";
	}
	else if (trueSailAngle < 158) {
		sailCardinal = "SouthEast";
	}
	else if (trueSailAngle < 203) {
		sailCardinal = "South";
	}
	else if (trueSailAngle < 248) {
		sailCardinal = "SouthWest";
	}
	else if (trueSailAngle < 293) {
		sailCardinal = "West";
	}
	else if (trueSailAngle < 338) {
		sailCardinal = "NorthWest";
	}
	else {
		sailCardinal = "North";
	}

	if (sailCardinal == wind)
	{
		sailMod = 1.5;
	}
	else if (sailCardinal == "North" && (wind == "NorthEast" || wind == "NorthWest"))
	{
		sailMod = 1.2;
	}
	else if (sailCardinal == "NorthEast" && (wind == "North" || wind == "East"))
	{
		sailMod = 1.2;
	}
	else if (sailCardinal == "East" && (wind == "NorthEast" || wind == "SouthEast"))
	{
		sailMod = 1.2;
	}
	else if (sailCardinal == "SouthEast" && (wind == "East" || wind == "South"))
	{
		sailMod = 1.2;
	}
	else if (sailCardinal == "South" && (wind == "SouthEast" || wind == "SouthWest"))
	{
		sailMod = 1.2;
	}
	else if (sailCardinal == "SouthWest" && (wind == "South" || wind == "West"))
	{
		sailMod = 1.2;
	}
	else if (sailCardinal == "West" && (wind == "SouthWest" || wind == "NorthWest"))
	{
		sailMod = 1.2;
	}
	else if (sailCardinal == "NorthWest" && (wind == "West" || wind == "North"))
	{
		sailMod = 1.2;
	}
	else if ((sailCardinal == "North" && wind == "South") || (sailCardinal == "South" && wind == "North"))
	{
		sailMod = 0.8;
	}
	else if ((sailCardinal == "NorthEast" && wind == "SouthWest") || (sailCardinal == "SouthWest" && wind == "NorthEast"))
	{
		sailMod = 0.8;
	}
	else if ((sailCardinal == "West" && wind == "East") || (sailCardinal == "East" && wind == "West"))
	{
		sailMod = 0.8;
	}
	else if ((sailCardinal == "SouthEast" && wind == "Northwest") || (sailCardinal == "Northwest" && wind == "SouthEast"))
	{
		sailMod = 0.8;
	}
	else
	{
		sailMod = 1;
	}
}

void ShipObject::FireCannon(bool direction)
{
	SDL_Point spawn;

	switch (ballSelected) {
	case 0:
		if (direction == true) {
			spawn.x = shipX + (sin((angle - 90) * M_PI / 180) * 15) + 50;

			spawn.y = shipY - (cos((angle - 90) * M_PI / 180) * 15) + 50;

			cannonBall.velocityX = (sin((angle - 90) * M_PI / 180) * 6 + (sin((angle)*M_PI / 180) * sailSetting) * sailMod);
			cannonBall.velocityY = (cos((angle - 90) * M_PI / 180) * 6 + (cos((angle)*M_PI / 180) * sailSetting) * sailMod);

			cannonBall.SetPosition(spawn);
			cannonBall.active = true;
		}
		else
		{
			spawn.x = shipX + (sin((angle + 90) * M_PI / 180) * 15) + 50;

			spawn.y = shipY - (cos((angle + 90) * M_PI / 180) * 15) + 50;

			cannonBall.velocityX = (sin((angle + 90) * M_PI / 180) * 6 + (sin((angle)*M_PI / 180) * sailSetting) * sailMod);
			cannonBall.velocityY = (cos((angle + 90) * M_PI / 180) * 6 + (cos((angle)*M_PI / 180) * sailSetting) * sailMod);

			cannonBall.SetPosition(spawn);
			cannonBall.active = true;
		}
		break;
	case 1:
		if (direction == true) {
			for (int i = 0; i < 3; i++) {
				spawn.x = shipX + (sin((angle - 90) * M_PI / 180) * 15) + 50;

				spawn.y = shipY - (cos((angle - 90) * M_PI / 180) * 15) + 50;

				fireBombs[i].velocityX = (sin((angle - 90 + fireBombs[i].angleMod) * M_PI / 180) * 6 + (sin((angle)*M_PI / 180) * sailSetting) * sailMod);
				fireBombs[i].velocityY = (cos((angle - 90 + fireBombs[i].angleMod) * M_PI / 180) * 6 + (cos((angle)*M_PI / 180) * sailSetting) * sailMod);

				fireBombs[i].SetPosition(spawn);
				fireBombs[i].active = true;
			}
		}
		else
		{
			for (int i = 0; i < 3; i++) {
				spawn.x = shipX + (sin((angle + 90) * M_PI / 180) * 15) + 50;

				spawn.y = shipY - (cos((angle + 90) * M_PI / 180) * 15) + 50;

				fireBombs[i].velocityX = (sin((angle + 90 + fireBombs[i].angleMod) * M_PI / 180) * 6 + (sin((angle)*M_PI / 180) * sailSetting) * sailMod);
				fireBombs[i].velocityY = (cos((angle + 90 + fireBombs[i].angleMod) * M_PI / 180) * 6 + (cos((angle)*M_PI / 180) * sailSetting) * sailMod);

				fireBombs[i].SetPosition(spawn);
				fireBombs[i].active = true;
			}
		}
		break;
	case 2:
		spawn.x = shipX + (sin((angle + 180) * M_PI / 180) * 15) + 50;

		spawn.y = shipY - (cos((angle + 180) * M_PI / 180) * 15) + 50;


		bomb.SetPosition(spawn);
		bomb.active = true;
		break;
	}

	
}

void ShipObject::LoadTextures(SDL_Renderer* passedRenderer) {
	shipSurf = IMG_Load("content/art/shipsheet.png");
	if (shipSurf == NULL)
	{
		SDL_Log("Null pointer returned while loading shipsheet.png");
		logData.push_back("Null pointer returned while loading shipsheet.png");
	}
	{
		SDL_Log("Loaded shipsheet.png successfully");
		logData.push_back("Loaded shipsheet.png successfully");
	}

	shipTexture = SDL_CreateTextureFromSurface(passedRenderer, shipSurf);
	if (shipTexture == NULL)
	{
		SDL_Log("Null pointer returned while converting surface to texture");
		logData.push_back("Null pointer returned while converting surface to texture");
	}
	{
		SDL_Log("shipTexture created successfully");
		SDL_FreeSurface(shipSurf);
		logData.push_back("shipTexture created successfully");
	}

	sailSurf = IMG_Load("content/art/sailsheet.png");
	if (sailSurf == NULL)
	{
		SDL_Log("Null pointer returned while loading sailsheet.png");
		logData.push_back("Null pointer returned while loading sailsheet.png");
	}
	{
		SDL_Log("Loaded sailsheet.png successfully");
		logData.push_back("Loaded sailsheet.png successfully");
	}

	sailTexture = SDL_CreateTextureFromSurface(passedRenderer, sailSurf);
	if (sailTexture == NULL)
	{
		SDL_Log("Null pointer returned while converting surface to texture");
		logData.push_back("Null pointer returned while converting surface to texture");
	}
	{
		SDL_Log("sailTexture created successfully");
		SDL_FreeSurface(sailSurf);
		logData.push_back("sailTexture created successfully");
	}
}

std::vector<const char*> ShipObject::sendLog() {
	return logData;
}

// ENEMY SHIP CODE

EnemyShip::EnemyShip(SDL_Renderer* passedRenderer) {

	LoadTextures(passedRenderer);

	sourceShipRect.h = 32;
	sourceShipRect.w = 32;
	sourceShipRect.x = sourceShipRect.w * sailSetting;
	sourceShipRect.y = 0;

	sourceSailRect.h = 16;
	sourceSailRect.w = 16;
	sourceSailRect.x = sourceSailRect.w * sailSetting;
	sourceSailRect.y = 0;

	sailR = 255;
	sailG = 255;
	sailB = 255;
	shipR = 99;
	shipG = 67;
	shipB = 39;
	sailRect.w = 50;
	sailRect.h = 50;
	shipRect.w = 100;
	shipRect.h = 100;

	shipRect.x = 1170;
	shipX = 1170.0;
	shipRect.y = 620;
	shipY = 620.0;

	sailSetting = 1;
	UpdateSail();

	health = 100;
	crewHealth = 100;

	for (int i = 0; i < 3; i++) {
		FireBomb tempBomb;
		tempBomb.Init(passedRenderer, ((i - 1) * 15));
		fireBombs.push_back(tempBomb);
	}
	bomb.Init(passedRenderer);

	cannonBall.Init(passedRenderer);
}

void EnemyShip::Update(int width, int height, const char* wind, SoundController* aM) {
	int whatDo;

	bool canFire = false;

	srand(time(NULL));

	whatDo = rand() % 5 + 1;

	

	if (SDL_GetTicks() > shipClock + 500) {
		shipClock = SDL_GetTicks();
		switch (whatDo) {
		case 1:
			if (turnL == false) {
				turnL = true;
			}
			else
			{
				turnL = false;
			}
			break;
		case 2:
			if (turnR == false) {
				turnR = true;
			}
			else
			{
				turnR = false;
			}
			break;
		case 3:
			if (sailSetting < 3)
			{
				sailSetting++;
				UpdateSail();
			}
			break;
		case 4:
			if (sailSetting > 1)
			{
				sailSetting--;
				UpdateSail();
			}
			break;
		case 5:
			ballSelected++;
			if (ballSelected > 2) {
				ballSelected = 0;
			}
			break;

		}

		switch (shootMode) {
		case 0:
			break;
		case 1:
			for (int i = 0; i < 3; i++) {
				if (fireBombs[i].active == false) {
					canFire = true;
				}
			}
			if (cannonBall.active == false && canFire == true && bomb.active == false) {
				FireCannon(true);
				aM->PlayCShot();
			}
			break;
		case 2:
			for (int i = 0; i < 3; i++) {
				if (fireBombs[i].active == false) {
					canFire = true;
				}
			}
			if (cannonBall.active == false && canFire == true && bomb.active == false) {
				FireCannon(false);
				aM->PlayCShot();
			}
			break;
		case 3:
			for (int i = 0; i < 3; i++) {
				if (fireBombs[i].active == false) {
					canFire = true;
				}
			}
			if (cannonBall.active == false && canFire == true && bomb.active == false) {
				FireCannon(true);
				aM->PlayCShot();
			}
			break;
		}

	}

	

	if (turnL == true) {
		angle = angle - (2 - sailSetting / 2);
	}
	if (turnR == true) {
		angle = angle + (2 - sailSetting / 2);
	}

	trueAngle = abs(fmod(angle, 360) + (angle / 360));



	if (sTurnL == true) {
		sailAngle = sailAngle - 2;
	}
	if (sTurnR == true) {
		sailAngle = sailAngle + 2;
	}
	if (angle - sailAngle > 90)
	{
		sailAngle = angle - 90;
	}
	else if (sailAngle - angle > 90)
	{
		sailAngle = angle + 90;
	}

	bool moveThisTurn = true;

	if ((frontLeft.x + (sin(angle * M_PI / 180))) > width + 30 || (frontRight.x + (sin(angle * M_PI / 180))) > width + 30) {
		shipRect.x = -30;
		shipX = -30;
		SDL_Log("Ship exited map right and was reset");
	}
	else if ((frontLeft.x - (sin(angle * M_PI / 180))) < -30 || (frontRight.x - (sin(angle * M_PI / 180))) < -30) {
		shipRect.x = width - 30;
		shipX = width - 30;
		SDL_Log("Ship exited map left and was reset");
	}
	if ((frontLeft.y + (cos(angle * M_PI / 180))) > height + 30 || (frontRight.y + (cos(angle * M_PI / 180))) > height + 30) {
		shipRect.y = -30;
		shipY = -30;
		SDL_Log("Ship exited map down and was reset");
	}
	else if ((frontLeft.y - (cos(angle * M_PI / 180))) < -30 || (frontRight.y - (cos(angle * M_PI / 180))) < -30) {
		shipRect.y = height - 30;
		shipY = height - 30;
		SDL_Log("Ship exited map up and was reset");
	}


	if (moveThisTurn != false) {

		calculateSailWind(wind);

		shipY = shipY - ((cos(angle * M_PI / 180) * sailSetting) * sailMod);
		shipX = shipX + ((sin(angle * M_PI / 180) * sailSetting) * sailMod);

		shipRect.y = round(shipY);
		shipRect.x = round(shipX);
	}

	sailRect.x = (shipRect.x + (shipRect.w / 2)) - (sailRect.w / 2);
	sailRect.y = (shipRect.y + (shipRect.h / 2)) - (sailRect.h / 2);


	//SDL_Log("Angle : %i", trueAngle);

	s1X = shipX + (sin((angle - 90) * M_PI / 180) * 33) + 50;
	s1Y = shipY - (cos((angle - 90) * M_PI / 180) * 33) + 50;

	s2X = shipX + (sin((angle + 90) * M_PI / 180) * 33) + 50;
	s2Y = shipY - (cos((angle + 90) * M_PI / 180) * 33) + 50;

	frontRight.y = shipY - (cos((angle + 22) * M_PI / 180) * 33) + 50;
	frontRight.x = shipX + (sin((angle + 22) * M_PI / 180) * 33) + 50;

	frontLeft.y = shipY - (cos((angle - 22) * M_PI / 180) * 33) + 50;
	frontLeft.x = shipX + (sin((angle - 22) * M_PI / 180) * 33) + 50;

	rearRight.y = shipY + (cos((angle - 22) * M_PI / 180) * 33) + 50;
	rearRight.x = shipX - (sin((angle - 22) * M_PI / 180) * 33) + 50;

	rearLeft.y = shipY + (cos((angle + 22) * M_PI / 180) * 33) + 50;
	rearLeft.x = shipX - (sin((angle + 22) * M_PI / 180) * 33) + 50;

	healthBar.x = shipX + (50 - (health / 4));
	healthBar.y = shipY + 15;
	healthBar.w = health / 2;
	healthBar.h = 2;

	crewHealthBar.x = shipX + (50 - (crewHealth / 4));
	crewHealthBar.y = shipY + 17;
	crewHealthBar.w = crewHealth / 2;
	crewHealthBar.h = 2;

}

void EnemyShip::LoadTextures(SDL_Renderer* passedRenderer){
	shipSurf = IMG_Load("content/art/shipsheet.png");
	if (shipSurf == NULL)
	{
		SDL_Log("Null pointer returned while loading shipsheet.png");
		logData.push_back("Null pointer returned while loading shipsheet.png");
	}
	{
		SDL_Log("Loaded shipsheet.png successfully");
		logData.push_back("Loaded shipsheet.png successfully");
	}

	shipTexture = SDL_CreateTextureFromSurface(passedRenderer, shipSurf);
	if (shipTexture == NULL)
	{
		SDL_Log("Null pointer returned while converting surface to texture");
		logData.push_back("Null pointer returned while converting surface to texture");
	}
	{
		SDL_Log("shipTexture created successfully");
		SDL_FreeSurface(shipSurf);
		logData.push_back("shipTexture created successfully");
	}

	sailSurf = IMG_Load("content/art/sailSheetEnemy.png");
	if (sailSurf == NULL)
	{
		SDL_Log("Null pointer returned while loading sailSheetEnemy.png");
		logData.push_back("Null pointer returned while loading sailSheetEnemy.png");
	}
	{
		SDL_Log("Loaded sailSheetEnemy.png successfully");
		logData.push_back("Loaded sailSheetEnemy.png successfully");
	}

	sailTexture = SDL_CreateTextureFromSurface(passedRenderer, sailSurf);
	if (sailTexture == NULL)
	{
		SDL_Log("Null pointer returned while converting surface to texture");
		logData.push_back("Null pointer returned while converting surface to texture");
	}
	{
		SDL_Log("sailTexture created successfully");
		SDL_FreeSurface(sailSurf);
		logData.push_back("sailTexture created successfully");
	}
}

// WIND CODE

WindCompass::WindCompass(SDL_Renderer* passedRenderer) {

	srand(time(NULL));

	windAngle = rand() % 8 + 1;

	switch (windAngle) {
	case 1:
		cardinal = "North";
		windAngle = 0;
		break;
	case 2:
		cardinal = "NorthEast";
		windAngle = 45;
		break;
	case 3:
		cardinal = "East";
		windAngle = 90;
		break;
	case 4:
		cardinal = "SouthEast";
		windAngle = 135;
		break;
	case 5:
		cardinal = "South";
		windAngle = 180;
		break;
	case 6:
		cardinal = "SouthWest";
		windAngle = 225;
		break;
	case 7:
		cardinal = "West";
		windAngle = 270;
		break;
	case 8:
		cardinal = "NorthWest";
		windAngle = 315;
		break;
	}
	SDL_Log("Wind heading %s", cardinal);

	windSource.h = 16;
	windSource.w = 16;
	windSource.x = 0;
	windSource.y = 0;

	windDest.x = 8;
	windDest.y = 8;
	windDest.w = 48;
	windDest.h = 48;

	createTextures(passedRenderer);

}

void WindCompass::createTextures(SDL_Renderer* passedRenderer) {
	compassSurf = IMG_Load("content/art/windcircle.png");
	if (compassSurf == NULL)
	{
		SDL_Log("Null pointer returned while loading windcircle.png");
	}
	{
		SDL_Log("Loaded windcircle.png successfully");
	}

	compassTexture = SDL_CreateTextureFromSurface(passedRenderer, compassSurf);
	if (compassTexture == NULL)
	{
		SDL_Log("Null pointer returned while converting surface to texture");
	}
	{
		SDL_Log("compassTexture created successfully");
		SDL_FreeSurface(compassSurf);
	}

	arrowSurf = IMG_Load("content/art/windarrow.png");
	if (arrowSurf == NULL)
	{
		SDL_Log("Null pointer returned while loading windarrow.png");
	}
	{
		SDL_Log("Loaded windarrow.png successfully");
	}

	arrowTexture = SDL_CreateTextureFromSurface(passedRenderer, arrowSurf);
	if (arrowTexture == NULL)
	{
		SDL_Log("Null pointer returned while converting surface to texture");
	}
	{
		SDL_Log("arrowTexture created successfully");
		SDL_FreeSurface(arrowSurf);
	}
}

void WindCompass::Render(SDL_Renderer* passedRenderer) {
	SDL_RenderCopyEx(passedRenderer, compassTexture, &windSource, &windDest, 0, new SDL_Point{24,24}, SDL_FLIP_NONE);
	SDL_RenderCopyEx(passedRenderer, arrowTexture, &windSource, &windDest, windAngle, new SDL_Point{ 24,24 }, SDL_FLIP_NONE);
}

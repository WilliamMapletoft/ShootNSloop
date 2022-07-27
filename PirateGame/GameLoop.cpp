#include "GameLoop.h"

Uint32 flags = SDL_WINDOW_RESIZABLE;

SDL_Window* window = SDL_CreateWindow("Shoot and Sloop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, flags);

SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

SoundController audioManager;

SDL_Event _event;

ShipObject ship(renderer);

EnemyShip enemy(renderer);

WindCompass compass(renderer);

Timer aTimer;

Timer aiTimer;

Timer coolDown;
const int DELTA_TIME = 16.5;

int W, H, gameTimer, score;

bool exitGame;

std::vector<const char*> logging;

// FONT SETUP

TTF_Font* gameFont = NULL;

SDL_Surface* timerText = NULL;

SDL_Texture* timerTexture = NULL;

SDL_Surface* scoreText = NULL;

SDL_Texture* scoreTexture = NULL;

SDL_Rect destText, destScore;

bool bordered, fullscreen;

//==============================================================================================

void HandleOneSecondInterval(void) {
    if (gameTimer > 0) {
        gameTimer -= 1;
    }
}

Uint32 TimerCallBackFunction(Uint32 interval, void* param) {
    HandleOneSecondInterval();
    return interval;
}

void GameLoop::Update() {

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


    // FONT SETUP

    if (TTF_Init() == -1) {
        SDL_Log("Error loading SDL_TTF");
        logging.push_back("Error loading SDL_TTF");
    }
    else
    {
        SDL_Log("SDL_TTF Loaded");
        logging.push_back("SDL_TTF Loaded");
    }

    // CALLING MAIN MENU FUNCTION

    audioManager.Pause();

    MainMenu();

    audioManager.Pause();

    // FINAL SETUP AND LOGGING

    gameFont = TTF_OpenFont("content/fonts/arial.ttf", 25);
    if (gameFont != NULL) {
        SDL_Log("Font loaded successfully");
        logging.push_back("Font loaded successfully");
    }
    else
    {
        SDL_Log("Font failed to load");
        logging.push_back("Font failed to load");
    }

    SDL_Color colour = { 255, 255, 255 };

    gameTimer = 60;

    // GAME TIMER START

    SDL_AddTimer(1000, &TimerCallBackFunction, NULL);

    // APPLYING DEBUFF FROM FILE
    // FILE READING

    std::ifstream file("content/Settings.txt");
    if (file.is_open() && file.good()) {
        std::string line;
        std::getline(file, line);
        try
        {
            int debuff = std::stoi(line);
            SDL_Log("Debuff file loaded and applied");
            logging.push_back("Debuff file loaded and applied");
            line.clear();
            ship.health -= debuff;
            ship.crewHealth -= debuff;
        }
        catch (const std::exception&)
        {
            SDL_Log("Debuff file unsuccessful, playing without");
            logging.push_back("Debuff file unsuccessful, playing without");
        }
    }
    file.close();


    // START OF GAMELOOP


	while (done != true) {

        if (gameTimer > 9) {
            destText.w = 70;
        }
        else
        {
            destText.w = 35;
        }

        destText.h = 50;
        destText.x = (W / 2) - (destText.w /2);
        destText.y = 0;

        std::string s = std::to_string(gameTimer);

        const char* text = s.c_str();

        timerText = TTF_RenderText_Solid(gameFont, text, colour);

        timerTexture = SDL_CreateTextureFromSurface(renderer, timerText);

        s = "Hits : " + std::to_string(score);

        text = s.c_str();

        destScore.w = 90;
        destScore.h = 30;
        destScore.x = destText.x - (destScore.w / 5);
        destScore.y = destText.y + (destScore.h * 2);

        scoreText = TTF_RenderText_Solid(gameFont, text, colour);

        scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreText);

        if (ship.health <= 0 || ship.crewHealth <= 0 || enemy.health <= 0 || enemy.crewHealth <= 0 || gameTimer <= 0) {
            done = true;
        }

        aTimer.resetTicksTimer();

        bool canFire = false;

        while (SDL_PollEvent(&_event))
        {

            if (_event.type == SDL_QUIT) { done = true; exitGame = true; }

            if (_event.type == SDL_KEYDOWN && _event.key.repeat == NULL)
            {
                switch (_event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    done = true;
                    exitGame = true;
                    break;
                case SDLK_m:
                    audioManager.Pause();
                    break;
                case SDLK_EQUALS:
                    audioManager.ChangeVolume(16);
                    break;
                case SDLK_MINUS:
                    audioManager.ChangeVolume(-16);
                    break;
                case SDLK_RIGHTBRACKET:
                    audioManager.ChangeMusic(16);
                    break;
                case SDLK_LEFTBRACKET:
                    audioManager.ChangeMusic(-16);
                    break;
                case SDLK_HASH:
                    audioManager.ChangeFX(16);
                    break;
                case SDLK_QUOTE:
                    audioManager.ChangeFX(-16);
                    break;
                case SDLK_a:
                    ship.turnL = true;
                    break;
                case SDLK_d:
                    ship.turnR = true;
                    break;
                case SDLK_q:
                    ship.sTurnL = true;
                    break;
                case SDLK_e:
                    ship.sTurnR = true;
                    break;
                case SDLK_t:
                    if (bordered == true) {
                        SDL_SetWindowBordered(window, SDL_FALSE);
                        bordered = false;
                    }
                    else
                    {
                        SDL_SetWindowBordered(window, SDL_TRUE);
                        bordered = true;
                    }
                    break;
                case SDLK_g:
                    if (fullscreen == true) {
                        SDL_SetWindowFullscreen(window, SDL_FALSE);
                        fullscreen = false;
                    }
                    else
                    {
                        SDL_SetWindowFullscreen(window, SDL_TRUE);
                        fullscreen = true;
                    }
                    break;

                case SDLK_r:
                    if (ship.ballSelected < 2) {
                        ship.ballSelected++;
                    }
                    else
                    {
                        ship.ballSelected = 0;
                    }
                    break;
                case SDLK_f:
                    if (ship.ballSelected > 0) {
                        ship.ballSelected--;
                    }
                    else {
                        ship.ballSelected = 2;
                    }
                    break;
                case SDLK_w:
                    if (ship.sailSetting < 3)
                    {
                        ship.sailSetting++;
                        ship.UpdateSail();
                    }
                    break;
                case SDLK_s:
                    if (ship.sailSetting > 0)
                    {
                        ship.sailSetting--;
                        ship.UpdateSail();
                    }
                    break;
                case SDLK_LEFT:
                    if (coolDown.getTicks() > 500) {
                        coolDown.resetTicksTimer();
                        for (int i = 0; i < 3; i++) {
                            if (ship.fireBombs[i].active == false) {
                                canFire = true;
                            }
                        }
                        if (ship.cannonBall.active == false && canFire == true && ship.bomb.active == false) {
                            ship.FireCannon(true);
                            audioManager.PlayCShot();
                        }
                    }
                    
                    break;
                case SDLK_RIGHT:
                    if (coolDown.getTicks() > 500) {
                        coolDown.resetTicksTimer();
                        for (int i = 0; i < 3; i++) {
                            if (ship.fireBombs[i].active == false) {
                                canFire = true;
                            }
                        }
                        if (ship.cannonBall.active == false && canFire == true && ship.bomb.active == false) {
                            ship.FireCannon(false);
                            audioManager.PlayCShot();
                        }
                    }
                    break;
                }
            
            }

            if (_event.type == SDL_KEYUP && _event.key.repeat == NULL)
            {
                switch (_event.key.keysym.sym) {
                case SDLK_a:
                    ship.turnL = false;
                    break;
                case SDLK_d:
                    ship.turnR = false;
                    break;
                case SDLK_q:
                    ship.sTurnL = false;
                    break;
                case SDLK_e:
                    ship.sTurnR = false;
                    break;
                }
            }
        }
       
        SDL_GetWindowSize(window, &W, &H);

        ship.Update(W, H, compass.cardinal);
        if (ship.cannonBall.active == true) {
            ship.cannonBall.Update();
        }
        for (int i = 0; i < 3; i++) {
            if (ship.fireBombs[i].active == true) {
                ship.fireBombs[i].Update();
            }
        }
        if (ship.bomb.active == true) {
            ship.bomb.Update();
        }

        enemy.Update(W, H, compass.cardinal, &audioManager);
        if (enemy.cannonBall.active == true) {
            enemy.cannonBall.Update();
        }
        for (int i = 0; i < 3; i++) {
            if (enemy.fireBombs[i].active == true) {
                enemy.fireBombs[i].Update();
            }
        }
        if (enemy.bomb.active == true) {
            enemy.bomb.Update();
        }

        CollisionDetect();
        
		Render(renderer);

        if (aTimer.getTicks() < DELTA_TIME)
        {
            SDL_Delay(DELTA_TIME - aTimer.getTicks());
        }
	}

    audioManager.Remove();

    

    if (exitGame != true) {
        SDL_Log("Gameloop ending");
        logging.push_back("Gameloop ending");
        Ending gameEnd;

        std::vector<const char*> temp;
        temp = ship.sendLog();
        for (int i = 0; i < temp.size() - 1; i++)
        {
            logging.push_back(temp[i]);
        }
        temp = enemy.sendLog();
        for (int i = 0; i < temp.size() - 1; i++)
        {
            logging.push_back(temp[i]);
        }

        int size = 0;
        std::ofstream outFile("content/logs/log.txt");
        for (int i = 0; i < logging.size(); i++) {
            size = strlen(logging[i]);
            outFile.write(logging[i], size);
            outFile.write("\n", 1);
            SDL_Log("Hi");
        }
        outFile.close();

        if (ship.health + ship.crewHealth > enemy.health + enemy.crewHealth) {
            gameEnd.Display(true, renderer, gameFont, W, H);
            SDL_Log("Player wins");
            logging.push_back("Player wins");
        }
        else
        {
            gameEnd.Display(false, renderer, gameFont, W, H);
            SDL_Log("Computer wins");
            logging.push_back("Computer wins");
        }
    }

    SDL_FreeSurface(timerText);
    SDL_DestroyTexture(timerTexture);
    TTF_CloseFont(gameFont);
}

void GameLoop::Render(SDL_Renderer* passedRenderer) {
	SDL_SetRenderDrawColor(passedRenderer, 15, 74, 168, 255);

	SDL_RenderClear(passedRenderer);

	ship.Render(passedRenderer); 

    enemy.Render(passedRenderer);

    compass.Render(renderer);

    LoadUI();

    SDL_RenderCopy(renderer, timerTexture, NULL, &destText);

    SDL_RenderCopy(renderer, scoreTexture, NULL, &destScore);

   // SDL_RenderDrawLine(passedRenderer, enemy.s1X, enemy.s1Y, enemy.s2X, enemy.s2Y);

	SDL_RenderPresent(passedRenderer);
}

void GameLoop::CollisionDetect() {
    double diffX, diffY, diffH, shipDiffX, shipDiffY, shipDiffH;

    shipDiffX = abs(ship.shipRect.x - enemy.shipRect.x);
    shipDiffY = abs(ship.shipRect.y - enemy.shipRect.y);

    shipDiffH = sqrt((shipDiffX * shipDiffX) + (shipDiffY * shipDiffY));

    if (shipDiffH < 40) {
        ship.shipX -= (enemy.shipX - ship.shipX) * 0.1;
        ship.shipY -= (enemy.shipY - ship.shipY) * 0.1;
        ship.health--;
        enemy.health--;
        SDL_Log("Ship collision detected");
        logging.push_back("Ship collision detected");
    }

    if (aiTimer.getTicks() > 500) {
        aiTimer.resetTicksTimer();
        if (shipDiffH < 400) {
            double tempX, tempX2, tempY, tempY2, tempH, tempH2;

            tempX = abs(ship.shipRect.x - enemy.s1X);
            tempY = abs(ship.shipRect.y - enemy.s1Y);
            tempH = sqrt((tempX * tempX) + (tempY * tempY));

            tempX2 = abs(ship.shipRect.x - enemy.s2X);
            tempY2 = abs(ship.shipRect.y - enemy.s2Y);
            tempH2 = sqrt((tempX2 * tempX2) + (tempY2 * tempY2));

            if (tempH < tempH2) {
                SDL_Log("Enemy within range to fire");
                enemy.shootMode = 1;
            }
            else {
                SDL_Log("Enemy within range to fire");
                enemy.shootMode = 2;
            }
        }
        else
        {
            enemy.shootMode = 0;
        }
    }
    

    if (ship.cannonBall.active == true) {
        diffX = abs(ship.cannonBall.destRect.x - (enemy.shipRect.x + 50));
        diffY = abs(ship.cannonBall.destRect.y - (enemy.shipRect.y + 50));

        diffH = sqrt((diffX * diffX) + (diffY * diffY));

        if (diffH < 50) {
            ship.cannonBall.active = false;
            ship.cannonBall.velocityX = 0;
            ship.cannonBall.velocityY = 0;
            enemy.health = enemy.health - 10;
            score++;
            SDL_Log("Enemy hit by cannonball");
            logging.push_back("Enemy hit by cannonball");
        }

        //SDL_Log("Distance : %f", diffH);
    }
    
    if (enemy.cannonBall.active == true) {
        diffX = abs(enemy.cannonBall.destRect.x - (ship.shipRect.x + 50));
        diffY = abs(enemy.cannonBall.destRect.y - (ship.shipRect.y + 50));

        diffH = sqrt((diffX * diffX) + (diffY * diffY));

        if (diffH < 50) {
            enemy.cannonBall.active = false;
            enemy.cannonBall.velocityX = 0;
            enemy.cannonBall.velocityY = 0;
            ship.health = ship.health - 10;
            SDL_Log("Player hit by cannonball");
            logging.push_back("Player hit by cannonball");
        }

        //SDL_Log("Distance : %f", diffH);
    }

    for (int i = 0; i < 3; i++) {
        if (ship.fireBombs[i].active == true) {
            diffX = abs(ship.fireBombs[i].destRect.x - (enemy.shipRect.x + 50));
            diffY = abs(ship.fireBombs[i].destRect.y - (enemy.shipRect.y + 50));

            diffH = sqrt((diffX * diffX) + (diffY * diffY));

            if (diffH < 50) {
                ship.fireBombs[i].active = false;
                ship.fireBombs[i].velocityX = 0;
                ship.fireBombs[i].velocityY = 0;
                enemy.crewHealth = enemy.crewHealth - 7;
                score++;
                SDL_Log("Enemy hit by firebomb");
                logging.push_back("Enemy hit by firebomb");
            }

            //SDL_Log("Distance : %f", diffH);
        }
    }

    for (int i = 0; i < 3; i++) {
        if (enemy.fireBombs[i].active == true) {
            diffX = abs(enemy.fireBombs[i].destRect.x - (ship.shipRect.x + 50));
            diffY = abs(enemy.fireBombs[i].destRect.y - (ship.shipRect.y + 50));

            diffH = sqrt((diffX * diffX) + (diffY * diffY));

            if (diffH < 50) {
                enemy.fireBombs[i].active = false;
                enemy.fireBombs[i].velocityX = 0;
                enemy.fireBombs[i].velocityY = 0;
                ship.crewHealth = ship.crewHealth - 7;
                SDL_Log("Player hit by firebomb");
                logging.push_back("Player hit by firebomb");
            }

            //SDL_Log("Distance : %f", diffH);
        }
    }

    if (ship.bomb.active == true && ship.bomb.blown == true) {
        diffX = abs(ship.bomb.destRect.x - (enemy.shipRect.x + 50));
        diffY = abs(ship.bomb.destRect.y - (enemy.shipRect.y + 50));

        diffH = sqrt((diffX * diffX) + (diffY * diffY));

        if (diffH < 100) {
            enemy.health = enemy.health - 25;
            enemy.crewHealth = enemy.crewHealth - 25;
            ship.bomb.active = false;
            ship.bomb.blown = false;
            score++;
            SDL_Log("Enemy hit by bomb");
            logging.push_back("Enemy hit by bomb");
        }
    }

    if (enemy.bomb.active == true && enemy.bomb.blown == true) {
        diffX = abs(enemy.bomb.destRect.x - (ship.shipRect.x + 50));
        diffY = abs(enemy.bomb.destRect.y - (ship.shipRect.y + 50));

        diffH = sqrt((diffX * diffX) + (diffY * diffY));

        if (diffH < 100) {
            ship.health = ship.health - 25;
            ship.crewHealth = ship.crewHealth - 25;
            enemy.bomb.active = false;
            enemy.bomb.blown = false;
            SDL_Log("Player hit by bomb");
            logging.push_back("Player hit by bomb");
        }
    }
}

void GameLoop::MainMenu() {
    TTF_Font* gameFont = NULL;

    gameFont = TTF_OpenFont("content/fonts/arial.ttf", 25);

    SDL_Color colour = { 255, 255, 255 };

    //===========================================================================================

    SDL_Surface* logoText = TTF_RenderText_Solid(gameFont, "Shoot and Sloop!", colour);

    SDL_Texture* logoTexture = SDL_CreateTextureFromSurface(renderer, logoText);

    //===========================================================================================

    SDL_Surface* ins1 = TTF_RenderText_Solid(gameFont, "Press space to start the game", colour);

    SDL_Texture* ins1Texture = SDL_CreateTextureFromSurface(renderer, ins1);

    //===========================================================================================

    SDL_Surface* ins2 = TTF_RenderText_Solid(gameFont, "A/D will turn your rudder", colour);

    SDL_Texture* ins2Texture = SDL_CreateTextureFromSurface(renderer, ins2);

    //===========================================================================================

    SDL_Surface* ins3 = TTF_RenderText_Solid(gameFont, "W/S will adjust sail length", colour);

    SDL_Texture* ins3Texture = SDL_CreateTextureFromSurface(renderer, ins3);

    //===========================================================================================

    SDL_Surface* ins4 = TTF_RenderText_Solid(gameFont, "Q/E will rotate sails", colour);

    SDL_Texture* ins4Texture = SDL_CreateTextureFromSurface(renderer, ins4);

    //===========================================================================================

    SDL_Surface* ins5 = TTF_RenderText_Solid(gameFont, "Left/Right will fire cannons", colour);

    SDL_Texture* ins5Texture = SDL_CreateTextureFromSurface(renderer, ins5);

    //===========================================================================================

    SDL_Surface* ins6 = TTF_RenderText_Solid(gameFont, "R/F will cycle shot type", colour);

    SDL_Texture* ins6Texture = SDL_CreateTextureFromSurface(renderer, ins6);

    //===========================================================================================

    SDL_Surface* ins7 = TTF_RenderText_Solid(gameFont, "Esc at any time will quit", colour);

    SDL_Texture* ins7Texture = SDL_CreateTextureFromSurface(renderer, ins7);

    //===========================================================================================

    SDL_Surface* ins8 = TTF_RenderText_Solid(gameFont, "+/- adjusts master volume", colour);

    SDL_Texture* ins8Texture = SDL_CreateTextureFromSurface(renderer, ins8);

    //===========================================================================================

    SDL_Surface* ins9 = TTF_RenderText_Solid(gameFont, "[/] adjusts music volume", colour);

    SDL_Texture* ins9Texture = SDL_CreateTextureFromSurface(renderer, ins9);

    //===========================================================================================

    SDL_Surface* ins10 = TTF_RenderText_Solid(gameFont, "'/# adjusts FX volume", colour);

    SDL_Texture* ins10Texture = SDL_CreateTextureFromSurface(renderer, ins10);

    //===========================================================================================

    SDL_Surface* ins11 = TTF_RenderText_Solid(gameFont, "t for borderless, g for fullscreen", colour);

    SDL_Texture* ins11Texture = SDL_CreateTextureFromSurface(renderer, ins11);

    //===========================================================================================

    SDL_Rect logoRect;

    if (gameFont != NULL) {
        SDL_Log("Font loaded successfully");
        logging.push_back("Font loaded successfully");
    }
    else
    {
        SDL_Log("Font loading failed");
        logging.push_back("Font loading failed");
    }


   

    bool mDone = false;

    while (!mDone) {

        SDL_GetWindowSize(window, &W, &H);

        SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);

        SDL_RenderClear(renderer);

        logoRect.w = 350;
        logoRect.h = 60;
        logoRect.x = (W / 2) - (logoRect.w /2);
        logoRect.y = (H / 4) - (logoRect.h / 2);

        SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect);

        // Game name

        logoRect.w = 310;
        logoRect.h = 30;
        logoRect.x = (W / 2) - (logoRect.w / 2);
        logoRect.y = (H / 3) - (logoRect.h / 2);

        SDL_RenderCopy(renderer, ins1Texture, NULL, &logoRect);

        // Space to start

        logoRect.w = 280;
        logoRect.h = 25;
        logoRect.x = (W / 2) - (logoRect.w / 2);
        logoRect.y = (H/3) + (logoRect.h);

        SDL_RenderCopy(renderer, ins2Texture, NULL, &logoRect);

        // A/D to turn rudder

        logoRect.w = 250;
        logoRect.h = 25;
        logoRect.x = (W / 2) - (logoRect.w / 2);
        logoRect.y = (H / 3) + (logoRect.h * 2);

        SDL_RenderCopy(renderer, ins3Texture, NULL, &logoRect);

        // W/S to adjust sail length

        logoRect.w = 200;
        logoRect.h = 25;
        logoRect.x = (W / 2) - (logoRect.w / 2);
        logoRect.y = (H / 3) + (logoRect.h * 3);

        SDL_RenderCopy(renderer, ins4Texture, NULL, &logoRect);

        // Q/E to rotate sails

        logoRect.w = 250;
        logoRect.h = 25;
        logoRect.x = (W / 2) - (logoRect.w / 2);
        logoRect.y = (H / 3) + (logoRect.h * 4);

        SDL_RenderCopy(renderer, ins5Texture, NULL, &logoRect);

        // Left / Right to fire cannons

        logoRect.w = 220;
        logoRect.h = 25;
        logoRect.x = (W / 2) - (logoRect.w / 2);
        logoRect.y = (H / 3) + (logoRect.h * 5);

        SDL_RenderCopy(renderer, ins6Texture, NULL, &logoRect);

        // R/F to cycle shot type

        logoRect.w = 250;
        logoRect.h = 25;
        logoRect.x = (W / 2) - (logoRect.w / 2);
        logoRect.y = (H / 3) + (logoRect.h * 6);

        SDL_RenderCopy(renderer, ins7Texture, NULL, &logoRect);

        // Esc at any time to quit  

        logoRect.w = 250;
        logoRect.h = 25;
        logoRect.x = (W / 2) - (logoRect.w / 2);
        logoRect.y = (H / 3) + (logoRect.h * 7);

        SDL_RenderCopy(renderer, ins8Texture, NULL, &logoRect);

        // +/- Master volume

        logoRect.w = 250;
        logoRect.h = 25;
        logoRect.x = (W / 2) - (logoRect.w / 2);
        logoRect.y = (H / 3) + (logoRect.h * 8);

        SDL_RenderCopy(renderer, ins9Texture, NULL, &logoRect);

        // [/] Music volume

        logoRect.w = 200;
        logoRect.h = 25;
        logoRect.x = (W / 2) - (logoRect.w / 2);
        logoRect.y = (H / 3) + (logoRect.h * 9);

        SDL_RenderCopy(renderer, ins10Texture, NULL, &logoRect);

        //'/# FX volume

        logoRect.w = 300;
        logoRect.h = 25;
        logoRect.x = (W / 2) - (logoRect.w / 2);
        logoRect.y = (H / 3) + (logoRect.h * 10);

        SDL_RenderCopy(renderer, ins11Texture, NULL, &logoRect);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&_event))
        {

            if (_event.type == SDL_QUIT) { 
                done = true; 
                mDone = true;
                exitGame = true;
            }

            if (_event.type == SDL_KEYDOWN && _event.key.repeat == NULL)
            {
                switch (_event.key.keysym.sym) {
                case SDLK_SPACE:
                    mDone = true;
                    break;
                case SDLK_ESCAPE:
                    done = true;
                    mDone = true;
                    exitGame = true;
                    break;
                case SDLK_t:
                    if (bordered == true) {
                        SDL_SetWindowBordered(window, SDL_FALSE);
                        bordered = false;
                    }
                    else
                    {
                        SDL_SetWindowBordered(window, SDL_TRUE);
                        bordered = true;
                    }
                    break;
                case SDLK_g:
                    if (fullscreen == true) {
                        SDL_SetWindowFullscreen(window, SDL_FALSE);
                        fullscreen = false;
                    }
                    else
                    {
                        SDL_SetWindowFullscreen(window, SDL_TRUE);
                        fullscreen = true;
                    }
                    break;
                }
            }
        }
    }
}

void GameLoop::LoadUI() {
    SDL_Rect destRect, selRect, place1, place2, place3;

    destRect.w = 150;
    destRect.h = 50;
    destRect.x = 0;
    destRect.y = H - (destRect.h);

    place1.w = 30;
    place1.h = 30;
    place1.x = destRect.x + 11;
    place1.y = destRect.y + 11;;
    place2.w = 30;
    place2.h = 30;
    place2.x = destRect.x + 61;
    place2.y = destRect.y + 11;
    place3.w = 30;
    place3.h = 30;
    place3.x = destRect.x + 111;
    place3.y = destRect.y + 11;

    selRect.w = 50;
    selRect.h = 50;
    selRect.x = destRect.x + (selRect.w * ship.ballSelected);
    selRect.y = destRect.y;

    SDL_SetRenderDrawColor(renderer, 7, 37, 84, 150);

    SDL_RenderFillRect(renderer, &destRect);

    SDL_RenderCopyEx(renderer, ship.cannonBall.cannonBallTexture, &ship.cannonBall.sourceRect, &place1, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, ship.fireBombs[1].cannonBallTexture, &ship.fireBombs[1].sourceRect, &place2, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, ship.bomb.bombTexture, &ship.bomb.sourceRect, &place3, 0, NULL, SDL_FLIP_NONE);

    SDL_SetRenderDrawColor(renderer, 219, 216, 0, 255);

    SDL_RenderDrawRect(renderer, &selRect);
}
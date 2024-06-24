#include "Game.hpp"
#include <iostream>
#include <fstream>

Game::Game(){
    music = LoadMusicStream("Sounds/music.ogg");
    explosionSound = LoadSound("Sounds/explosion.ogg");
    PlayMusicStream(music);
    InitGame();
}

Game::~Game(){
    Alien::UnloadImages();
    UnloadMusicStream(music);
    UnloadSound(explosionSound);
}

void Game::Update(){
    if(run){
        double currentTime = GetTime();
        if(currentTime - timeLastSpawn > mysteryShipSpawnInterval){
            mysteryship.Spawn();
            timeLastSpawn = GetTime();
            mysteryShipSpawnInterval = GetRandomValue(10, 20);
        }



        for(auto& laser: player1.lasers){
            laser.Update();
        }

        MoveAliens();
        AlienShootLaser();
        for(auto& laser: alienLasers){
            laser.Update();
        }

        mysteryship.Update();
        CheckForColision(); 

        DeleteInactiveLasers();
    }else if(IsKeyDown(KEY_ENTER)){
        Reset();
        InitGame();
    }
}

void Game::Draw(){
    player1.Draw();

    for(auto& laser: player1.lasers){
        laser.Draw();
    }

    for(auto& obstacle: obstacles){
        obstacle.Draw();
    }

    for(auto& alien: aliens){
        alien.Draw();
    }
    
    for(auto& laser: alienLasers){
        laser.Draw();
    }

    mysteryship.Draw();
}

void Game::HandleInput(){
    if(run){
        if(IsKeyDown(KEY_LEFT)) {
        kleft = true;
        } 

        if(IsKeyUp(KEY_LEFT)) {
            kleft = false;
        }

        if(IsKeyDown(KEY_RIGHT)) {
            kright = true;
        } 

        if(IsKeyUp(KEY_RIGHT)) {
            kright = false;
        }

        if(kleft) {
            player1.MoveLeft();
        }
        if(kright) {
            player1.MoveRigth();
        }
        if(IsKeyDown(KEY_SPACE)) {
            player1.ShotBullets();
        }
        if(IsKeyDown(KEY_FIVE)){
            GameOver();
        }
    }
}

void Game::Difficulty(int diff) {
    difficulty = diff;
    switch (difficulty) {
        case 0:  // Facil
            alienLaserShootInterval = 1.0f;
            mysteryShipSpawnInterval = GetRandomValue(20, 30);
            break;
        case 1:  // Media
            alienLaserShootInterval = 0.75f;
            mysteryShipSpawnInterval = GetRandomValue(15, 25);
            break;
        case 2:  // Dificil
            alienLaserShootInterval = 0.35f;
            mysteryShipSpawnInterval = GetRandomValue(10, 20);
            break;
        case 3:  // Infierno
            alienLaserShootInterval = 0.10f;
            mysteryShipSpawnInterval = GetRandomValue(5, 10);
            break;
    }
}

void Game::DeleteInactiveLasers()
{
    for(auto it = player1.lasers.begin(); it != player1.lasers.end();){
        if(!it -> active){
            it = player1.lasers.erase(it);
        }else{
            ++it;
        }
    }

    for(auto it = alienLasers.begin(); it != alienLasers.end();){
        if(!it -> active){
            it = alienLasers.erase(it);
        }else{
            ++it;
        }
    }
}

std::vector<Obstacle> Game::CreateObstacles()
{
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth() - (4*obstacleWidth))/5;

    for(int i = 0; i<4; i++){
        float offsetX= (i+1) * gap+i * obstacleWidth;
        obstacles.push_back(Obstacle({offsetX, float(GetScreenWidth()-200)}));
    }
    return obstacles;
}

std::vector<Alien> Game::CreateAliens()
{
    std::vector<Alien> aliens;
    for(int row=0; row< 5; row++){
        for(int column =0; column < 11; column++){
            int alienType;
            if(row == 0){
                alienType =3;
            }else if(row==1 || row ==2){
                alienType = 2;
            }else{
                alienType =1;
            }
            float x = 60 + column * 55;
            float y = 110 + row * 55;
            aliens.push_back(Alien(alienType, {x, y}));
        }
    }
    return aliens;
}


void Game::MoveAliens(){
    for(auto& alien: aliens){
        if(alien.position.x + alien.alienImages[alien.type-1].width > GetScreenWidth()-25){
            AliensDirection = -1;
            MoveDownAliens(4);
        }else if(alien.position.x < 25){
            AliensDirection = 1;
            MoveDownAliens(4);
        }
        alien.Update(AliensDirection);
    }
}

void Game::MoveDownAliens(int distance)
{
    for(auto& alien: aliens){
        alien.position.y += distance;
    }
}

void Game::AlienShootLaser()
{
    double CurrentTime = GetTime();
    if(CurrentTime - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty()){
        int randomIndex = GetRandomValue(0, aliens.size()-1);
        Alien& alien = aliens[randomIndex];
        alienLasers.push_back(Laser({alien.position.x + alien.alienImages[alien.type-1].width/2, alien.position.y + alien.alienImages[alien.type-1].height}, 6));

        timeLastAlienFired = GetTime();
    }

}

void Game::CheckForColision()
{
    //spaceship lasers

    for(auto& laser: player1.lasers){
        auto it = aliens.begin();
        while(it != aliens.end()){
            if(CheckCollisionRecs(it -> getRect(), laser.getRect())){
                PlaySound(explosionSound);
                if(it -> type ==1){
                    score += 100;
                }else if(it -> type == 2){
                    score += 200;
                }else if(it -> type ==3){
                    score += 300;
                }
                checkForScore();
                it = aliens.erase(it);
                laser.active=false;
            }else{
                ++it;
            }
        }

        for(auto& obstacle: obstacles){
        auto it = obstacle.blocks.begin();
        while(it!= obstacle.blocks.end()){
            if(CheckCollisionRecs(it -> getRect(), laser.getRect())){
                it = obstacle.blocks.erase(it);
                laser.active = false;
    
            }else{
                ++it;
            }
        }

        for(auto& Alienlaser: alienLasers){
            if(CheckCollisionRecs(laser.getRect(), Alienlaser.getRect())){
                Alienlaser.active = false;
                laser.active = false;
            }
        }
    }

    if(CheckCollisionRecs(mysteryship.getRect(), laser.getRect())){
        mysteryship.alive = false;
        laser.active =false;
        score += 500;
        checkForScore();
        PlaySound(explosionSound);
    }

    }

    //alien laser

    for(auto& laser: alienLasers){
        if(CheckCollisionRecs(laser.getRect(), player1.getRect())){
            laser.active = false;
            lives--;
            if(lives==0){
                GameOver();
            }
        }

        for(auto& obstacle: obstacles){
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()){
                if(CheckCollisionRecs(it -> getRect(), laser.getRect())){
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                }else{
                    ++it;
                }

            }
        }

    }

    //alien collisio with obstacle

    for(auto& alien: aliens){
        for(auto& obstacle: obstacles){
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()){
                if(CheckCollisionRecs(it -> getRect(), alien.getRect())){
                    it = obstacle.blocks.erase(it);
                }else{
                    it++;
                }
            }
        }
        if(CheckCollisionRecs(alien.getRect(), player1.getRect())){
            GameOver();

        }
    }
}

void Game::GameOver()
{
    
    run = false;
}

void Game::InitGame()
{
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    AliensDirection = 1;
    timeLastAlienFired = 0.0;
    timeLastSpawn = 0.0;
    Difficulty(difficulty);
    kleft = false;
    kright = false;
    lives = 3;
    run = true;
    score = 0;
    highscore = loadHighScoreFromFile();

}

void Game::checkForScore()
{
    if(score > highscore){
        highscore = score;
        SaveHighScore(highscore);
    }
}

void Game::SaveHighScore(int highscore)
{
    std::ofstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open()){
        highscoreFile << highscore;
        highscoreFile.close();
    }else{
        std::cout << "Failed to open file" << std::endl;
    }

}

int Game::loadHighScoreFromFile()
{
    int loeadedHighScore =0;
    std::ifstream highScoreFile("highscore.txt");
    if(highScoreFile.is_open()){
        highScoreFile >> loeadedHighScore;
        highScoreFile.close();
    }else{
        std::cerr << "Failed to load highscore form file." << std::endl;
    
    }
    return loeadedHighScore;
}

void Game::Reset(){
    player1.Reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();
}

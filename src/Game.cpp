#include "Game.hpp"
#include <iostream>


Game::Game(){
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    AliensDirection = 1;
    timeLastAlienFired = 0.0;
    timeLastSpawn = 0.0;
    mysteryShipSpawnInterval = GetRandomValue(10, 20);
}

Game::~Game(){
    Alien::UnloadImages();
}

void Game::Update(){

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
    if(IsKeyDown(KEY_LEFT)){
        player1.MoveLeft();
    }else if(IsKeyDown(KEY_RIGHT)){
        player1.MoveRigth();
    }else if(IsKeyDown(KEY_SPACE)){
        player1.ShotBullets();
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
        obstacles.push_back(Obstacle({offsetX, float(GetScreenWidth()-100)}));
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
        if(alien.position.x + alien.alienImages[alien.type-1].width > GetScreenWidth()){
            AliensDirection = -1;
            MoveDownAliens(4);
        }else if(alien.position.x < 0){
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
    }

    }

    //alien laser

    for(auto& laser: alienLasers){
        if(CheckCollisionRecs(laser.getRect(), player1.getRect())){
            laser.active = false;
            std::cout << "Spaceship hit" << std::endl;
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
}

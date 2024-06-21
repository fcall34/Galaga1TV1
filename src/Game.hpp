#pragma once
#include "spaceship.hpp"
#include "Obstacle.hpp"
#include "Alien.hpp"
#include "Mysteryship.hpp"

class Game{
    public:
    Game();
    ~Game();

    void Draw();
    void Update();
    void HandleInput();
    bool run;
    int lives;

    private:
        void DeleteInactiveLasers();
        std::vector<Obstacle> CreateObstacles();
        std::vector<Alien> CreateAliens();
        void MoveAliens();
        void MoveDownAliens(int distance);
        void AlienShootLaser();
        void CheckForColision();
        void GameOver();
        void Reset();
        void InitGame();
        Spaceship player1;
        std::vector<Obstacle> obstacles;
        std::vector<Alien> aliens;
        int AliensDirection;
        std::vector<Laser> alienLasers;
        constexpr static float alienLaserShootInterval = 0.35;
        float timeLastAlienFired;
        Mysteryship mysteryship;
        float mysteryShipSpawnInterval;
        float timeLastSpawn;
        bool kleft, kright;
        
};
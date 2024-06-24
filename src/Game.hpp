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
    void Difficulty(int diff);
    void Pause();
    bool run = true;
    bool pause = false;
    int lives;
    int score;
    int highscore;
    int difficulty;
    

    Music music;


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
        void checkForScore();
        void SaveHighScore(int highscore);
        int loadHighScoreFromFile();  
        Spaceship player1;
        std::vector<Obstacle> obstacles;
        std::vector<Alien> aliens;
        int AliensDirection;
        std::vector<Laser> alienLasers;
        float alienLaserShootInterval;
        float timeLastAlienFired;
        Mysteryship mysteryship;
        float mysteryShipSpawnInterval;
        float timeLastSpawn;
        bool kleft, kright;
        Sound explosionSound;
        
};
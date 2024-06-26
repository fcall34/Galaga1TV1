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
    void Reset();
    bool run;
    bool pause;
    bool win;
    int lives;
    int score;
    int highscore;
    int difficulty;
    

    Music music;


    private:
        void DeleteInactiveLasers();
        std::vector<Alien> CreateAliens();
        std::vector<Obstacle> CreateObstacles();
        void MoveAliens();
        void MoveDownAliens(int distance);
        void AlienShootLaser();
        void CheckForColision();
        void GameOver();
        void InitGame();
        void checkForScore();
        void SaveHighScore(int highscore);
        void Win();
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
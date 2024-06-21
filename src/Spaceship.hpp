#pragma once
#include <raylib.h>
#include "Laser.hpp"
#include <vector>


class Spaceship{
    public:
        int speed;
        Spaceship();
        ~Spaceship();

        void Draw();
        void MoveLeft();
        void MoveRigth();
        void ShotBullets();
        Rectangle getRect();
        void Reset();
        int getPosX();
        int getPosY();

        std::vector<Laser> lasers;
    private:
        Texture2D image;
        Vector2 position;
        double lastFireTime;
};
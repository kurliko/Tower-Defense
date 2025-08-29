#pragma once
#include <SFML/Graphics.hpp>
#include "Enemy.hpp"


struct Wave
{      
    Wave(){}
    Wave(int c, float s ,int h) : count(c), speed(s), hp(h){}
    int count;
    float speed;
    int hp;
    float spawnDelay=0.4f;;
};

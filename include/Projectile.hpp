#pragma once
#include <SFML/Graphics.hpp>
#include "Enemy.hpp"

class Projectile {
public:
    Projectile(sf::Vector2f start, Enemy* target, float speed, int damage);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool isDestroyed() const { return destroyed; }

private:
    sf::CircleShape shape;
    Enemy* target;
    float speed;
    int damage;
    bool destroyed;
};

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

class Enemy {
public:
    Enemy(const std::vector<sf::Vector2f>& path, float speed, int hp);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition()const;
    void takeDamage(int dmg);
    bool isDead() const { return hp <= 0; }
    bool reachedEnd() const ;


private:
    sf::CircleShape shape;
    std::vector<sf::Vector2f> path;
    size_t pathIndex;
    float speed;
    int hp;
};
